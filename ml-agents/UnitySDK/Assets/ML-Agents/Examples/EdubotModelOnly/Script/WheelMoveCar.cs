using UnityEngine;
using System.Collections;

public class WheelMoveCar : MonoBehaviour
{
    public WheelCollider colliderRR;
    public WheelCollider colliderRL;
    public int VerTorque1;
    public int HorTorque2;
    public Collider ballCastCollider;

    // 바퀴 회전을 위한 Transform
    public Transform wheelTransformRL;
    public Transform wheelTransformRR;

    // 감속량
    public float decSpeed = 1000f;
    public Rigidbody rb;

    // Use this for initialization
    void Start()
    {
        rb = GetComponent<Rigidbody>(); 
        VerTorque1 = 100;
        HorTorque2 = 100;

        // originnal mass value = 0.0 , 0.8 , 0.8 
        rb.centerOfMass = new Vector3((float)0.0 , (float)0.0 , (float)0.8 );

    }

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
       float speed = rb.velocity.sqrMagnitude;

        if (speed > 5)
        {
            VerTorque1 = 0;
        }
        else
        {
            VerTorque1 = 100;
        }

       if (((Input.GetAxis("Vertical") != 0) || (Input.GetAxis("Horizontal") != 0)))
        {
            colliderRR.motorTorque = VerTorque1 * Input.GetAxis("Vertical") - HorTorque2 * Input.GetAxis("Horizontal") ;
            colliderRL.motorTorque = VerTorque1 * Input.GetAxis("Vertical") + HorTorque2 * Input.GetAxis("Horizontal") ;

            colliderRR.brakeTorque = 0;
            colliderRL.brakeTorque = 0;
            ballCastCollider.material.dynamicFriction = (float)0.15;
            ballCastCollider.material.staticFriction = (float)0.15;
        }
        else 
        {
            colliderRR.motorTorque = 0;
            colliderRL.motorTorque = 0;

            colliderRR.brakeTorque = decSpeed;
            colliderRL.brakeTorque = decSpeed;
            ballCastCollider.material.dynamicFriction = 100;
            ballCastCollider.material.staticFriction = 100;

        }

        //colliderRR.steerAngle = 0;
        //colliderRL.steerAngle = 0;

        UpdateMeshesPostion();       

        UnityEngine.Debug.Log("inputt " + Input.GetAxis("Vertical") + "," + Input.GetAxis("Vertical"));
        UnityEngine.Debug.Log("motorTorque " + colliderRR.motorTorque + "," + colliderRL.motorTorque
            +"_"+ colliderRR.brakeTorque + "," + colliderRL.brakeTorque + "_" 
            + speed);

    }

    void UpdateMeshesPostion()
    {
        //for (int i = 0; i < 4; i++)
        {
            Quaternion quat;
            Vector3 pos;

            colliderRR.GetWorldPose(out pos, out quat);
            wheelTransformRR.position = pos;
            wheelTransformRR.rotation = quat;

            colliderRL.GetWorldPose(out pos, out quat);
            wheelTransformRL.position = pos;
            wheelTransformRL.rotation = quat;

        }
    }
}
