struct QUAT {
	float x;
	float y;
	float z;
	float w;
} ;

struct EULER {
	float roll;
	float pitch;
	float yaw;
} ;

typedef struct VECT3D {
	float x;
	float y;
	float z;
} ;

typedef struct EBMDATA {
	int id;             // ¼¾¼­ID
	int time_stamp;			// ÂïÈù time
	QUAT quat;          // quaternion
	//EULER euler;        // eulerAngle (degree)
	//VECT3D gyro;        // gyroscope (dps)
	//VECT3D acc;         // accelerometer (g)
	//VECT3D acc_no_g;    // accelerometer without gravity (g)
	//VECT3D mag;         // magnetometer (uT)
	//float temp;         // ¼¾¼­¿Âµµ (¼·¾¾)
	//int batt;           // ¹èÅÍ¸®ÀÜ·®(%)
} ;