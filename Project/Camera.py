from picamera import PiCamera
import time



camera = PiCamera()
camera.resolution = (640, 480)
camera.vflip = True
camera_record = False


def Record():
	
	
	camera.start_recording("my.h264")
	camera.start_preview()


def stop_Recording():
	camera.stop_preview()
	camera.stop_recording()
	


Record()
time.sleep(5)
stop_Recording()
	


