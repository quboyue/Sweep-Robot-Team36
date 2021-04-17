using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class test : MonoBehaviour {

	public Quaternion testt =new Quaternion(1,0,0,0);

	// Use this for initialization
	void Start () {
		print(this.transform.localRotation);
		this.transform.localRotation = testt;
		print(this.transform.localRotation);

	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
