using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RightMoveCar : MonoBehaviour
{
    public Rigidbody rb;
    // 바퀴 회전을 위한 Transform
    public Transform wheelTransformRL;
    public Transform wheelTransformRR;
    public float VerTorque1;
    public float HorTorque2;
    public float Meshwheelspeed;
    public GameObject edubot;
    public WheelCollider colliderRR;
    public WheelCollider colliderRL;
    public Collider ballCastCollider;
    public Collider edubotbase;
    public float speed;

    // 감속량
    public float decSpeed = 1000f;

    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody>();
        //edubot = GetComponent<GameObject>();
        VerTorque1 = 0;
        HorTorque2 = 0;

        // originnal mass value = 0.0 , 0.8 , 0.8 
        rb.centerOfMass = new Vector3((float)0.0, (float)0.0, (float)0.8);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            Application.Quit();
        }
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        Vector3 dirToGo = Vector3.zero;
        Vector3 rotateDir = Vector3.zero;
        dirToGo = transform.forward;
        rotateDir = transform.up;

        speed = rb.velocity.sqrMagnitude;

        if (speed > 5)
        {
            VerTorque1 = (float)0;
            
        }
        else
        {
            VerTorque1 = (float)0.5;
            HorTorque2 = (float)0.5;
        }

        if (((Input.GetAxis("Vertical") != 0) || (Input.GetAxis("Horizontal") != 0)))
        {

            //if (Input.GetAxis("Horizontal") != 0) { 
            edubot.transform.RotateAround(edubotbase.bounds.center, rotateDir, Input.GetAxis("Horizontal") * HorTorque2);

            //edubot.transform.Rotate(rotateDir * Input.GetAxis("Horizontal") * HorTorque2, Time.deltaTime * 150f);
            //}
            //else if (Input.GetAxis("Vertical") != 0)
            //{
                rb.AddForce(dirToGo * VerTorque1 * Input.GetAxis("Vertical") * VerTorque1, ForceMode.VelocityChange);
            //}           

            ballCastCollider.material.staticFriction = (float)0.15;
            colliderRR.brakeTorque = 0;
            colliderRL.brakeTorque = 0;
            ballCastCollider.material.dynamicFriction = (float)0.15;
            ballCastCollider.material.staticFriction = (float)0.15;
            Meshwheelspeed = 2;
        }
        else
        {
            VerTorque1 = 0;
            HorTorque2 = 0;

            edubot.transform.Rotate(rotateDir * HorTorque2, Time.deltaTime * 150f);
            rb.AddForce(dirToGo * VerTorque1, ForceMode.VelocityChange);

            colliderRR.brakeTorque = decSpeed;
            colliderRL.brakeTorque = decSpeed;
            ballCastCollider.material.dynamicFriction = 100;
            ballCastCollider.material.staticFriction = 100;
            Meshwheelspeed = 0;
        }

        UpdateMeshesPostion();

        //UnityEngine.Debug.Log("inputt " + Input.GetAxis("Vertical") + "," + Input.GetAxis("Vertical"));
        //UnityEngine.Debug.Log("motorTorque " + speed);
    }

    void UpdateMeshesPostion()
    {
        /*Vector3 rotateDir = Vector3.zero;
        rotateDir = transform.*Meshwheelspeed;        
        wheelTransformRL.transform.Rotate(rotateDir);
        wheelTransformRR.transform.Rotate(rotateDir);*/
        // 바퀴회전효과      

        wheelTransformRL.Rotate(speed * 50 * Time.fixedDeltaTime, 0, 0);
        wheelTransformRR.Rotate(speed * 50 * Time.fixedDeltaTime, 0, 0);
    }
}
