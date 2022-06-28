using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using rclcs;

public class OnCollision : MonoBehaviourRosNode
{
    public string NodeName = "OnColUnity";
    public string Topic = "col_flag";
    protected override string nodeName { get { return NodeName; } }
    private Publisher<std_msgs.msg.Bool> OnCol;
    private std_msgs.msg.Bool msg;


    protected override void StartRos()
    {
        OnCol = node.CreatePublisher<std_msgs.msg.Bool>(Topic);
        msg = new std_msgs.msg.Bool();
    }


    void OnCollisionEnter(Collision col)
    {
        if (col.gameObject.name != "Floor")
        {
            Debug.Log("Collision");
            msg.Data = true;
            OnCol.Publish(msg);
        }
        
    }

}