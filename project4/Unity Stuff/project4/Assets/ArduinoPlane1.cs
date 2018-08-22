using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using UnityEngine;
public class ArduinoPlane1 : MonoBehaviour
{
    SerialPort serial = new SerialPort("COM9", 115200);
    // Use this for initialization
    void Start()
    {
        serial.Open();
    }
    // Update is called once per frame
    void Update()
    {
        if (!serial.IsOpen)
        {
            serial.Open();
        }
        string[] acc = serial.ReadLine().Split(',');
        string[] gyr = serial.ReadLine().Split(',');
        string[] mag = serial.ReadLine().Split(',');
        Quaternion rot = Quaternion.Euler(new Vector3(float.Parse(gyr[0])*10.0f, float.Parse(gyr[1])*10.0f, float.Parse(gyr[2])*10.0f));
        transform.rotation = Quaternion.Slerp(transform.rotation, rot, Time.deltaTime * 2.0f);
    }
}
