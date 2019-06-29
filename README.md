
# image shot
<img src="/picture/1.png" width="70%" height="70%">
<img src="/picture/2.png" width="70%" height="70%">

# 클릭하면 유투브 연결  
Click image to link to YouTube video.  
[![Video Label](http://img.youtube.com/vi/IynnlFMelIc/0.jpg)](https://youtu.be/IynnlFMelIc?t=0s)  

# 유니티 Simulation 프로잭트 구성  
* RigidScenes.unity  
  Edubot 바퀴가 아닌 Object 자체를 움직여서 이동시킨 방식을 사용  
  움직이는 모습은 동일하지만 실제 바퀴를 움직여 기체를 움직이는 현실 세계 방식을 사용하지 않음.  
* WheelScenes.unity  
  Unity의 Wheel Collider 이용하여 현실 세계의 구조와 동일하게 바퀴의 이동으로 Edubot 기체를 움직임.  
  Wheel Collider 는 지상 차량용 특수 콜라이더입니다.   
  Wheel Collider는 NVIDIA PhysX SDK 3.4.0 가 사용 되었습니다.   
  
* Control  
  W Key  = 전진  
  S Key  = 후진  
  A Key  = 왼쪽 회전  
  D Key  = 오른쪽 회전  
  
* Etc  
  그외에 3D 영상 배경은 unity asset에서 무료 모델 사용 하였습니다.   
  해당배경이 적용된 Scenes의 경우 파일이 너무 커서 Git에는 올리 수 없습니다.   
  그외에 거리센서, 바닦센서는 적용 되지 않았습니다.   
