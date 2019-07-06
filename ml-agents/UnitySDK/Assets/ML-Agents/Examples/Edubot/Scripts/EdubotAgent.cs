using System.Collections.Generic;
using UnityEngine;
using MLAgents;

public class EdubotAgent : Agent
{
    Rigidbody rBody;
    //public float speed = 10;

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
    //public float speed;

    // 감속량
    public float decSpeed = 1000f;

    void Start()
    {
        rBody = GetComponent<Rigidbody>();
        VerTorque1 = 0;
        HorTorque2 = 0;
        // originnal mass value = 0.0 , 0.8 , 0.8 
        rBody.centerOfMass = new Vector3((float)0.0, (float)0.0, (float)0.8);
    }

    public Transform Target;
    public override void AgentReset()
    {
        if (this.transform.position.y < 0)
        {
            // If the Agent fell, zero its momentum
            this.rBody.angularVelocity = Vector3.zero;
            this.rBody.velocity = Vector3.zero;
            this.transform.position = new Vector3(0, 0.5f, 0);
        }

        // Move the target to a new spot
        /*Target.position = new Vector3(Random.value * 8 - 4,
                                      0.5f,
                                      Random.value * 8 - 4);*/
        Target.position = new Vector3(Random.value * 16 - 8,
                                      0.5f,
                                      Random.value * 16 - 8);
    }

    public override void CollectObservations()
    {
        // Target and Agent positions
        AddVectorObs(Target.position);
        AddVectorObs(this.transform.position);

        // Agent velocity
        AddVectorObs(rBody.velocity.x);
        AddVectorObs(rBody.velocity.z);
    }

    public override void AgentAction(float[] vectorAction, string textAction)
    {
        // Actions, size = 2
        Vector3 controlSignal = Vector3.zero;
        Vector3 dirToGo = Vector3.zero;
        Vector3 rotateDir = Vector3.zero;
        dirToGo = transform.forward;
        rotateDir = transform.up;

        /*controlSignal.x = vectorAction[0];
        controlSignal.z = vectorAction[1];
        rBody.AddForce(controlSignal * speed);*/

        UnityEngine.Debug.Log("controlSignal");
        if (((vectorAction[0] != 0) || (vectorAction[1] != 0)))
        {
            VerTorque1 = (float)0.5;
            HorTorque2 = (float)5;

            edubot.transform.RotateAround(edubotbase.bounds.center, rotateDir, vectorAction[0]  * HorTorque2);
            rBody.AddForce(dirToGo * vectorAction[1] * VerTorque1, ForceMode.VelocityChange);
          

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
            rBody.AddForce(dirToGo * VerTorque1, ForceMode.VelocityChange);

            colliderRR.brakeTorque = decSpeed;
            colliderRL.brakeTorque = decSpeed;
            ballCastCollider.material.dynamicFriction = 100;
            ballCastCollider.material.staticFriction = 100;
            Meshwheelspeed = 0;
        }





        // Rewards
        float distanceToTarget = Vector3.Distance(this.transform.position,
                                                  Target.position);

        // Reached target
        if (distanceToTarget < 1.42f)
        {
            SetReward(1.0f);
            Done();
        }

        // Fell off platform
        if (this.transform.position.y < -0.5)
        {
            Done();
        }

    }

}

