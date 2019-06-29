using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraFollowMe : MonoBehaviour
{
    public Transform target;
    public float dist = 1.0f;
    public float height =1.0f;
    public float smmothRotate = 5.0f;

    private Transform tr;

    //Use this for initialization
    private void Start()
    {
        tr = GetComponent<Transform>();
    }

    //Excute When Update Func is ended.
    private void LateUpdate()
    {
        // 부드러운 회전을 위한 Mathf.LerpAngle
        float currYAngle = Mathf.LerpAngle(tr.eulerAngles.y, target.eulerAngles.y,
            smmothRotate * Time.deltaTime);

        // 오일러 타입을 쿼터니언으로 바꾸기 
        Quaternion rot = Quaternion.Euler(0, currYAngle, 0);
        //Vector3.back  -> Vector3(0, 0, -1) z forward
        //Vector3.right -> Vector3(1, 0, 0)  x left
        //Vector3.up    -> Vector3(0, 1, 0)  y down
        //카메라 위치를 타겟 회전 각도만큼 회전 후 dist만 큼 띄우고, 높이를 올리기
        //tr.position = target.position - (rot * Vector3.forward * dist) + (Vector3.up * height);
        tr.position = target.position - (Vector3.forward * 5) - (Vector3.right * 8) + (Vector3.up * 3);

        //타켓을 바라보게 하기 
        tr.LookAt(target);
    }
}

