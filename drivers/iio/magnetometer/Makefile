#
# Makefile for industrial I/O Magnetometer sensor drivers
#

obj-$(CONFIG_AK8975)	+= ak8975.o
obj-$(CONFIG_HID_SENSOR_MAGNETOMETER_3D) += hid-sensor-magn-3d.o
<<<<<<< HEAD
obj-$(CONFIG_SENSORS_YAS_MAGNETOMETER)	+= yas_mag_drv-yas532.o yas_mag_kernel.o
=======
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83

obj-$(CONFIG_IIO_ST_MAGN_3AXIS) += st_magn.o
st_magn-y := st_magn_core.o
st_magn-$(CONFIG_IIO_BUFFER) += st_magn_buffer.o

obj-$(CONFIG_IIO_ST_MAGN_I2C_3AXIS) += st_magn_i2c.o
obj-$(CONFIG_IIO_ST_MAGN_SPI_3AXIS) += st_magn_spi.o
<<<<<<< HEAD

obj-y += yas537/
CFLAGS_yas_mag_kernel.o += -Idrivers/iio
=======
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
