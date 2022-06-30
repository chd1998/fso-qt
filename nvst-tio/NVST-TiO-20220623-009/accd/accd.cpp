#include "accd/accd.h"
#include "atlib/atcore.h"
#include "atlib/atutility.h"
#include "mainwindow.h"
#include <QLibrary>
#include <QThread>
#include <QDebug>
#include <QEventLoop>
#include <QDir>
#include <string>
#include <cstring>
#include "cfitsio/fitsio.h"

aCCD::aCCD(QObject *parent):QThread(parent)
{
    int ret=init_aCCD();
    if (ret != 0)
    {
        qDebug("Failed in initializing CCD...");
    }
    else
    {
        qDebug("Succeeded in initializing CCD...");
    }
}
int aCCD::init_aCCD()
{
    int i_retCode;
    double temperature = 0;
    //initialize CCD
    i_retCode = AT_InitialiseLibrary();
    AT_InitialiseUtilityLibrary();
    //AT_64 iNumberDevices = 0;
    //i_retCode = AT_GetInt(AT_HANDLE_SYSTEM, L"DeviceCount", &iNumberDevices);
    //iNumberDevices=iNumberDevices-2;
    //qDebug("Found %lld device(s)...", iNumberDevices);
    if (realDev >=1 ) {
        //open the Camera
        if (AT_Open(ccdID, &handle)==AT_SUCCESS)
        {
            qDebug("Succeeded in Opening CCD");
            ccdM="Succeeded in Opening CCD";
            //start cooler to temperature for simcamera
            AT_SetBool(handle, L"SensorCooling", AT_TRUE);
            AT_GetFloat(handle, L"SensorTemperature", &temperature);
            AT_SetEnumString(handle,L"ElectronicShutteringMode",L"Rolling");
            //for Zyla Cam
            //AT_SetBool(handle, L"RollingShutterGlobalClear", AT_TRUE);
            //AT_SetEnumString(handle,L"PixelReadoutRate",L"100 MHz");
            AT_SetEnumeratedString(handle, L"TriggerMode",L"Internal");
            AT_SetBool(handle, L"Overlap", true);
            //AT_SetEnumeratedString(handle, L"CycleMode", L"Continuous");
            //AT_SetFloat(handle,L"FrameRate",double(frameRate));

            qDebug("Temperature： %f", temperature);
            qDebug()<<"Cooler ON...";
            AT_SetEnumString(handle, L"PixelEncoding", L"Mono16");
            AT_SetEnumString(handle, L"SimplePreAmpGainControl",L"16-bit (low noise & high well capacity)");
            AT_GetInt(handle, L"AOI Height", &imgH);
            AT_GetInt(handle, L"AOI Width", &imgW);
            AT_GetInt(handle, L"AOI Stride",  &imgStride);
            AT_SetInt(handle, L"AOIHBin", 1);
            AT_SetInt(handle, L"AOILeft", 1);
            AT_SetInt(handle, L"AOIVBin", 1);
            AT_SetInt(handle, L"AOITop", 1);
            AT_WC c0[255] = {0};
            AT_WC c1[255] = {0};
            AT_GetString(handle,L"CameralModel",c0,256);
            AT_GetString(handle,L"CameraName",c1,256);
            CamM=QString::fromWCharArray(c0);
            CamN=QString::fromWCharArray(c1);
            //AT_GetInt(handle,L"AccumulateCount",&f0);
            //f1=f0;
            qDebug("image parameters: %lld %lld %lld", imgW,imgH,imgStride);
            i_retCode=0;
            CamM="TiO";
            ccdM=QString::number(realDev)+" "+CamM+" "+CamN+" Device(s) Found...";
            //localsave=true;
        }
        else
        {
            qDebug("Failed in Opening CCD");
            ccdM="Failed in Opening CCD";
            i_retCode=1;
        }
    }
    else
    {
        //qDebug("found %lld %s %s device...", iNumberDevices,QString::toStdString(CamM),);
        ccdM="Found "+QString::number(realDev)+" "+CamM+"-"+CamN+" Device(s)...";
        i_retCode=1;
    }
    return i_retCode;
}

void aCCD::run()
{
    //int lt1,lt2;
    qDebug("aCCD Thread Started...");
    ccdM="aCCD Thread Started...";
    //AT_SetFloat(handle,L"FrameRate", frameRate);
    serialNo=0;
    //AT_SetEnumString(handle, L"CycleMode", L"Continuous");
    //AT_Command(handle, L"AcquisitionStart");
    ccdM=" ";
    //t1=QDateTime::currentDateTime();
    //lt1=t1.toSecsSinceEpoch();  //获取当前时间戳
    //bool localfirst=true;
    //localsave=true;
    while (!isInterruptionRequested())
    {
        if(live)
        {

            getData();


        }
    }
    AT_Command(handle, L"AcquisitionStop");
    AT_Flush(handle);
    AT_Close(handle);
    AT_Close(handle);
    AT_FinaliseLibrary();
    AT_FinaliseUtilityLibrary();
    ccdM="aCCD Thread Stopped...";
    qDebug("aCCD Thread Stopped...");
}

void aCCD::getData()
{
    //QMutex fitslock;
    //fitslock.lock();
    //savefits_locked=true;
    AT_SetFloat(handle, L"ExposureTime", expTime/1000.0);
    AT_GetInt(handle, L"ImageSizeBytes", &imageSizeBytes);
    int bufferSize = static_cast<int>(imageSizeBytes);
    buffer = new unsigned char[bufferSize];
    bufferback=buffer;
    //qDebug("1.5.1");
    uint buflen=imgW*imgH;
    unpackedBuffer = new unsigned short[imgW * imgH];
    //uint len = (sizeof(unpackedBufferback)/sizeof(unpackedBufferback[0]));
    unpackedBufferback=unpackedBuffer;
    unpackedBuffer01=new unsigned short[imgW * imgH];
    unpackedBufferback01=unpackedBuffer01;
    //Declare the number of buffers and the number of frames interested in
    int NumberOfBuffers = numBuffer;
    int NumberOfFrames= frameRate+1;
    //Allocate a number of memory buffers to store frames
    unsigned char** AcqBuffers = new unsigned char*[NumberOfBuffers];
    unsigned char** AlignedBuffers = new unsigned char*[NumberOfBuffers];
    for (int i=0; i < NumberOfBuffers; i++) {
     AcqBuffers[i] = new unsigned char[bufferSize + 7];
     AlignedBuffers[i] = reinterpret_cast<unsigned char*>((uintptr_t(AcqBuffers[i% NumberOfBuffers]) + 7) & ~7);
    }
    AT_Flush(handle);
    for(int i=0; i < NumberOfBuffers; i++) {
     AT_QueueBuffer(handle, AlignedBuffers[i], bufferSize);
    }

    AT_SetEnumString(handle, L"CycleMode", L"Continuous");

    int BufSize;

    //dir example
    //e:\20211008\TIO\dark\062905\062905
    //e:\20211008\TIO\FLAT00
    //e:\20211008\TIO\12882\071655\071655

    QDateTime current_date_time =QDateTime::currentDateTimeUtc();
    current_date_d =current_date_time.toString("yyyyMMdd");
    current_date_t2 =current_date_time.toString("hhmmss");
    if(savefits && !diskfull )
    {
        if(fpre=="T" && localsave)
        {
            saveDir=savepre+"\\"+current_date_t2;
            if(!sdir.exists(saveDir) && localsave)
                sdir.mkpath(saveDir);
        }
        if(fpre=="dark")
        {
            //saveDir=savepre+"\\"+current_date_t;
            saveDir=savepred;
            if(!sdir.exists(saveDir))
                sdir.mkpath(saveDir);
            //acqcnt=1000;
        }
        if(fpre=="FLAT")
        {
            //QString fcnt = QString("%1").arg(flatcnt, 2, 10, QLatin1Char('0'));
            //QString fcnt = current_date_t2;
            saveDir=savepref+current_date_t1;
            QDir *fdir = new QDir(saveDir);
            QDir tmpdir(saveDir);
            QStringList filter;
            filter<<"*.fits";
            fdir->setNameFilters(filter);
            QFileInfoList fileInfoList=fdir->entryInfoList(filter);
            if(fserialNo == 0 && tmpdir.exists(saveDir) && fileInfoList.count() != datanum)
            {               
                tmpdir.removeRecursively();
            }

            if(fileInfoList.count() == datanum  && display)
                emit stop_Acq();

            if(!sdir.exists(saveDir))
              sdir.mkpath(saveDir);

            delete fdir;
        }

    }
    qDebug()<<"Saving Pre: "<<savepred<<" "<<savepref<<" "<<savepre;
    qDebug()<<"Saving Dir: "<<saveDir;

    AT_Command(handle, L"AcquisitionStart");
    int saveStatus;
    if(localsave && fpre=="T")
    {
        t1=QDateTime::currentDateTime();
        lt1=t1.toSecsSinceEpoch();  //获取当前时间戳
    }

    for (int i=0; i < NumberOfFrames; i++) {
        //if btnSanp pressed during readout loop, break it to restart
        if(localfirst)
        {
            break;
        }
         AT_WaitBuffer(handle, &buffer, &BufSize, AT_INFINITE);
         //Application specific data processing goes here..
         AT_WC pixelEncoding[255] = {0};
         int pixelEncodingIndex;
         AT_GetEnumIndex(handle, L"PixelEncoding", &pixelEncodingIndex);
         AT_GetEnumStringByIndex(handle, L"PixelEncoding", pixelEncodingIndex, pixelEncoding, 255);

         AT_ConvertBuffer(buffer, reinterpret_cast<unsigned char *>(unpackedBuffer), imgW, imgH, imgStride, pixelEncoding, L"Mono16");

         //qDebug()<<"Saving : "<<QString::number(fserialNo);
         saveStatus=1;
         if(savefits && i >= 1 && !diskfull ){
           if(fpre=="T")
           {
               if(localsave)
               {
                    saveStatus=saveData(saveDir,unpackedBufferback);
               }
           }
           else{
                   if(fpre=="FLAT" || fpre=="dark")
                   {
                        saveStatus=saveData(saveDir,unpackedBufferback);
                   }
           }
           if(saveStatus==0)
           {
               serialNo=(serialNo+1) % frameRate; //single cycle
               fserialNo=fserialNo+1; //total number
               qDebug()<<"Saving : "<<saveDir<<" "<<QString::number(fserialNo);
               if(!continousACQ){
                   if( fpre=="T" && (fserialNo - datanum)==0 && fserialNo>0)
                   {
                       emit stop_Acq();
                       break;
                   }
               }
               if((fpre=="FLAT" && (fserialNo - datanum)==0 && fserialNo>0) || (fpre=="dark" && (fserialNo - datanum)==0 && fserialNo>0))
               {
                   emit stop_Acq();
                   break;
               }
           }//end of savestatus

         }//end of savefits

         //if(NULL != unpackedBufferback)
         //{
            //if(live && !display_locked && display)
            //if(live && display)
        if(live && !display_locked && display)
            emit buf_Ready(unpackedBufferback,buflen);
        //if(!histcalc_locked && live)
        //{

        //emit calcHist(unpackedBufferback,buflen);
        histfirst=false;
        //}
         //}
         //Re-queue the buffers
         AT_QueueBuffer(handle, AlignedBuffers[i%NumberOfBuffers], bufferSize);
         //if btnSanp pressed during readout loop, break it to restart
         if(localfirst)
         {
             break;
         }
         //if not localsave--->break and start new acq
         if(!localsave && fpre=="T")
         {
             t2=QDateTime::currentDateTime();
             lt2=t2.toSecsSinceEpoch();  //获取当前时间戳
             dt=lt2-lt1;
             if(dt>=groupdelay)
             {
                 localsave=true;
                 break;
             }

         }

        }//end of num. of frames,for loop

    if(localsave && !localfirst && fpre=="T")
    {
        t2=QDateTime::currentDateTime();
        lt2=t2.toSecsSinceEpoch();  //获取当前时间戳
        dt=lt2-lt1;
        if(dt>=groupdelay)
        {
            localsave=true;
        }
        else
        {
            localsave=false;
            //wait_Acq=true;
            //lt1=lt2;
            //emit pause_Acq(dt);
        }
    }

    //Stop the acquisition
    AT_Command(handle, L"AcquisitionStop");
    AT_Flush(handle);

    //Free the allocated buffer
    for (int i=0; i < NumberOfBuffers; i++) {
     delete[] AcqBuffers[i];
     //AcqBuffers[i]=NULL;
    }
    delete[] AlignedBuffers;
    //AlignedBuffers=NULL;
    delete[] AcqBuffers;
    //AcqBuffers=NULL;

    delete[] unpackedBufferback;
    //delete[] unpackedBufferback01;
    delete[] bufferback;

    //saveDir="";
    if(freedisk<=3)
    {
        diskfull=true;
        savefits=false;
        emit stop_Acq();
        //break;
    }else
        diskfull=false;
    //fitslock.unlock();
    //savefits_locked=false;
    if(localfirst)
        localfirst=false;
}

int aCCD::saveData(QString savePath,unsigned short* buff)
{
    QString fstr;

    int status = 0; /* initialize status before calling fitsio routines 0-ok 1-failed*/

    if(fpre=="FLAT" || fpre=="dark")
        fstr = QString("%1").arg(fserialNo, 6, 10, QLatin1Char('0'));
    else
        fstr = QString("%1").arg(serialNo, 6, 10, QLatin1Char('0'));
    //int fserialNo=printf("%6d",serialNo);
    QString fn=savePath+"\\"+"T_"+fstr+".fits";
    char *fitsname;
    QByteArray tmp=fn.toLatin1();
    fitsname=tmp.data();
    fitsfile *fptr;
    long fpixel = 1, naxis = 2, nelements;
    long naxes[2] = { long(imgW), long(imgH) };
    int bitpix=USHORT_IMG;
    float exposure;
    /* create new fits file */
    fits_create_file(&fptr, fitsname, &status);
    if(status == 0)
    {
        //qDebug("create file ok! %d", status);
        fits_create_img(fptr, bitpix, naxis, naxes, &status);
        /* Write a keyword; must pass the ADDRESS of the value */
        exposure = expTime;
        fits_update_key(fptr, TFLOAT, "EXPOSURE", &exposure,"Exposure Time in ms", &status);
        tmpchar=objname.toLatin1();
        fkword=tmpchar.data();
        fits_update_key(fptr, TSTRING, "OBSOBJ", fkword,"NOAA, Solar activity region", &status);
        QDateTime current_date_time =QDateTime::currentDateTimeUtc();
        current_date_d =current_date_time.toString("yyyy-MM-dd");
        current_date_t3 =current_date_time.toString("hh:mm:ss");
        QString obsdate=current_date_d+"T"+current_date_t3;
        tmpchar=obsdate.toLatin1();
        fkword=tmpchar.data();
        fits_update_key(fptr, TSTRING, "DATE_OBS", fkword,"Date of Observation",&status);
        QString obscor=obscor1+obscor2;
        tmpchar=obscor.toLatin1();
        fkword=tmpchar.data();
        fits_update_key(fptr, TSTRING, "OBSCOOD", fkword,"Coordinate of Observation",&status);
        QString telescope="FSO NVST";
        tmpchar=telescope.toLatin1();
        fkword=tmpchar.data();
        fits_update_key(fptr, TSTRING, "TELESCOP", fkword,"Telescope",&status);
        QString instrument="HRIS";
        tmpchar=instrument.toLatin1();
        fkword=tmpchar.data();
        fits_update_key(fptr, TSTRING, "INSTRUME", fkword,"High Resolution Imaging System",&status);
        QString aosys="off";
        tmpchar=aosys.toLatin1();
        fkword=tmpchar.data();
        fits_update_key(fptr, TSTRING, "AOSYS", fkword,"AO SYS ON/OFF",&status);
        int wavelen=7058;
        fits_update_key(fptr, TINT, "WAVELEN", &wavelen,"Wavelength of Observation",&status);
        float passband=10.0000;
        fits_update_key(fptr, TFLOAT, "PASSBAND", &passband,"Passband in angstrom",&status);
        int offband=0;
        fits_update_key(fptr, TINT, "OFFBAND", &offband,"OffBand in angstrom",&status);
        float focallength=33.5;
        fits_update_key(fptr, TFLOAT, "FOCALLEN", &focallength,"Focal length in meter",&status);
        float cdelt1=0.052;
        float cdelt2=0.052;
        fits_update_key(fptr, TFLOAT, "CDELT1", &cdelt1,"Image scale arcsec/pixel in X",&status);
        fits_update_key(fptr, TFLOAT, "CDELT2", &cdelt2,"Image scale arcsec/pixel in Y",&status);
        float xpix=6.5000;
        float ypix=6.5000;
        fits_update_key(fptr, TFLOAT, "XPIXSZ", &xpix,"Pixel width in microns",&status);
        fits_update_key(fptr, TFLOAT, "YPIXSZ", &ypix,"Pixel height in microns",&status);
        int xbin=1;
        int ybin=1;
        fits_update_key(fptr, TINT, "XBINING", &xbin,"Binning factor in width",&status);
        fits_update_key(fptr, TINT, "YBINING", &ybin,"Binning factor in height",&status);
        nelements = imgW*imgH; /* number of pixels to write */
        /* Write the array of integers to the image */
        fits_write_img(fptr, TUSHORT, fpixel, nelements, buff, &status);
        /*if(status==0)
        {
           serialNo=(serialNo+1) % frameRate;
           fserialNo=fserialNo+1;
        }*/
        //qDebug("Saving  %s... %d",qPrintable(fn),status);
        fits_close_file(fptr, &status); /* close the file */

    }
    else
    {
        //qDebug("create %s file failed! %d",fitsname,status);
        status=1;
    }

    return status;
}
