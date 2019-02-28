package com.danpe.facedetection;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.net.Uri;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;


public class MainActivity extends Activity {
    private static final int SELECT_IMAGE = 1;
    private static final int REQUEST_EXTERNAL_STORAGE = 1;
    private static final String TAG = "MainActivity";
    private static final String modelPath = "facedetection";

    private Bitmap yourSelectedImage = null;

    private TextView infoResult;
    private ImageView imageView;
    private FaceDetection faceDetector = new FaceDetection();


    private static String[] PERMISSIONS_STORAGE = {
            "android.permission.READ_EXTERNAL_STORAGE",
            "android.permission.WRITE_EXTERNAL_STORAGE"};


    public static void verifyStoragePermissions(Activity activity) {
        try {
            //检测是否有写的权限
            int permission = ActivityCompat.checkSelfPermission(activity,
                    "android.permission.WRITE_EXTERNAL_STORAGE");
            if (permission != PackageManager.PERMISSION_GRANTED) {
                // 没有写的权限，去申请写的权限，会弹出对话框
                ActivityCompat.requestPermissions(activity, PERMISSIONS_STORAGE, REQUEST_EXTERNAL_STORAGE);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        verifyStoragePermissions(this);

        //拷贝模型到 sd 卡
        try {
            copyBigDataToSD("det1.bin");
            copyBigDataToSD("det2.bin");
            copyBigDataToSD("det3.bin");
            copyBigDataToSD("det1.param");
            copyBigDataToSD("det2.param");
            copyBigDataToSD("det3.param");
            copyBigDataToSD("VanFace.bin");
            copyBigDataToSD("VanFace.param");
        } catch (IOException e) {
            e.printStackTrace();
        }
        //模型初始化
        File sdDir = Environment.getExternalStorageDirectory();//获取跟目录
        String sdPath = sdDir.toString() + "/" + modelPath;
        Log.i(TAG, sdPath);
        faceDetector.LoadModels(sdPath);

        infoResult = (TextView) findViewById(R.id.infoResult);
        imageView = (ImageView) findViewById(R.id.imageView);
        Button buttonImage = (Button) findViewById(R.id.buttonImage);
        buttonImage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View arg0) {
                Intent i = new Intent(Intent.ACTION_PICK);
                i.setType("image/*");
                startActivityForResult(i, SELECT_IMAGE);
            }
        });

        Button buttonDetect = (Button) findViewById(R.id.buttonDetect);
        buttonDetect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View arg0) {
                if (yourSelectedImage == null)
                    return;

                //检测流程
                int width = yourSelectedImage.getWidth();
                int height = yourSelectedImage.getHeight();
                byte[] imageDate = getPixelsRGBA(yourSelectedImage);

                long timeDetectFace = System.currentTimeMillis();
                faceDetector.SetThreadsNumber(4);
                int[] detectResult = faceDetector.Detect(imageDate, height, width);
                timeDetectFace = System.currentTimeMillis() - timeDetectFace;
                Log.i(TAG, "检测时间：" + timeDetectFace);

                if (detectResult.length > 1) {
                    int faceNum = detectResult[0];
                    Log.i(TAG, "图宽：" + width + "高：" + height + " 人脸数目：" + faceNum);
                    infoResult.setText("图宽：" + width + "高：" + height + "检测时间：" + timeDetectFace + " 数目：" + faceNum);
                    Bitmap drawBitmap = yourSelectedImage.copy(Bitmap.Config.ARGB_8888, true);

                    Canvas canvas = new Canvas(drawBitmap);
                    Paint paint = new Paint();
                    paint.setColor(Color.RED);
                    paint.setStyle(Paint.Style.STROKE);//不填充
                    paint.setStrokeWidth(5);  //线的宽度
                    for (int i = 0; i < faceNum; i++) {
                        int left, top, right, bottom;

                        left = detectResult[1 + 150 * i];
                        top = detectResult[2 + 150 * i];
                        right = detectResult[3 + 150 * i];
                        bottom = detectResult[4 + 150 * i];

                        canvas.drawRect(left, top, right, bottom, paint);
                        //画特征点
                        for (int j = 0; j < 68; j++) {
                            canvas.drawPoint(detectResult[15 + 150 * i + 2 * j], detectResult[15 + 150 * i + 2 * j + 1], paint);
                        }
                    }
                    imageView.setImageBitmap(drawBitmap);
                } else {
                    infoResult.setText("未检测到人脸");
                }
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        faceDetector.DeleteModels();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (resultCode == RESULT_OK && null != data) {
            Uri selectedImage = data.getData();

            try {
                if (requestCode == SELECT_IMAGE) {
                    Bitmap bitmap = decodeUri(selectedImage);
                    yourSelectedImage = bitmap.copy(Bitmap.Config.ARGB_8888, true);

                    imageView.setImageBitmap(yourSelectedImage);
                }
            } catch (FileNotFoundException e) {
                Log.e("MainActivity", "FileNotFoundException");
            }
        }
    }

    private Bitmap decodeUri(Uri selectedImage) throws FileNotFoundException {
        // Decode image size
        BitmapFactory.Options o = new BitmapFactory.Options();
        o.inJustDecodeBounds = true;
        BitmapFactory.decodeStream(getContentResolver().openInputStream(selectedImage), null, o);

        // The new size we want to scale to
        final int REQUIRED_SIZE = 400;

        // Find the correct scale value. It should be the power of 2.
        int width_tmp = o.outWidth, height_tmp = o.outHeight;
        int scale = 1;
        while (true) {
            if (width_tmp / 2 < REQUIRED_SIZE || height_tmp / 2 < REQUIRED_SIZE) {
                break;
            }
            width_tmp /= 2;
            height_tmp /= 2;
            scale *= 2;
        }

        // Decode with inSampleSize
        BitmapFactory.Options o2 = new BitmapFactory.Options();
        o2.inSampleSize = scale;
        return BitmapFactory.decodeStream(getContentResolver().openInputStream(selectedImage), null, o2);
    }

    //提取像素点
    private byte[] getPixelsRGBA(Bitmap image) {
        // calculate how many bytes our image consists of
        int bytes = image.getByteCount();
        ByteBuffer buffer = ByteBuffer.allocate(bytes); // Create a new buffer
        image.copyPixelsToBuffer(buffer); // Move the byte data to the buffer
        byte[] temp = buffer.array(); // Get the underlying array containing the

        return temp;
    }

    private void copyBigDataToSD(String strOutFileName) throws IOException {
        Log.i(TAG, "start copy file " + strOutFileName);
        File sdDir = Environment.getExternalStorageDirectory();//获取根目录
        File file = new File(sdDir.toString() + "/" + modelPath);
        if (!file.exists()) {
            file.mkdir();
        }

        String tmpFile = sdDir.toString() + "/" + modelPath + "/" + strOutFileName;
        File f = new File(tmpFile);
        if (f.exists()) {
            Log.i(TAG, "file exists " + strOutFileName);
            return;
        }
        InputStream myInput;
        java.io.OutputStream myOutput = new FileOutputStream(sdDir.toString() + "/" + modelPath + "/" + strOutFileName);
        myInput = this.getAssets().open(strOutFileName);
        byte[] buffer = new byte[1024];
        int length = myInput.read(buffer);
        while (length > 0) {
            myOutput.write(buffer, 0, length);
            length = myInput.read(buffer);
        }
        myOutput.flush();
        myInput.close();
        myOutput.close();
        Log.i(TAG, "end copy file " + strOutFileName);
    }

}
