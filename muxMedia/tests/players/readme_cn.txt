
             ���ز���sampleʹ��˵��

1���������
��ִ�г�������sample_localplay������һ���ļ�����xxx.mp3��ִ���������£�
  ./sample_localplay /filepath/xxx.mp3

��ѡ���������£�
  -s ָ�������Ļ�ļ����������£�
  ./sample_localplay /filepath/xxx.mp3 -s /filepath/xxx1.lrc,/filepath/xxx2.lrc
  
����һ����������(����Ŀ¼����
  ./sample_localplay bluray:/����Ŀ¼·��/?playlist=-1
  
����һ������������ISO��ʽ��,���������У�
->����ISO
  mkdir /tmp/avatar/;mount -o loop -t udf /mnt/usb/3DBLURAYISO.COM_AVATAR_3D.iso  /tmp/avatar/
->���Ź��غ������Ŀ¼
  ./sample_localplay bluray:/tmp/avatar/?playlist=-1

2�����Ź����г��ò������
    h: ���������Ϣ
    play: ����
    pause: ��ͣ
    seek: seek������seek time_offset(���ļ���ͷ��ʱ�䣬��λ�����룬milliseconds��
    stop: ֹͣ
    resume: �ָ�
    bw: ���ˣ�bw 2 2���ٶȿ���
    ff: �����ff 2 2���ٶȿ��
    info: �����ǰ��������Ϣ
	set sync 3000 0 0:������Ƶ��ͬ�������ͺ�3000ms(��Ƶpts��������3000ms)
    set sync 0 3000 0:������Ƶ��ͬ�������ͺ�3000ms(��Ƶpts��������3000ms)
	set sync 0 0 3000:������Ļ��ͬ�������ͺ�3000ms(��Ļpts��������3000ms)
	set mute:���þ���
	set vol  20�����������ȼ�20(0~100)
	set vmode 0: ����HDMI��Ƶģʽ��0-2Dģʽ��1-3D FramePacking,2-3D SideBySide, 3-3D TopAndBottom,����ֵ��ο����ͷ�ļ���
	set id 0 0 1 0:�������л���4���������α�ʾ��ĿID����Ƶ��ID����Ƶ��ID,��Ļ��ID��
	
3��HiPlayer���Ǹ߰��汾��֧��playready����ʹ��ʱ��Ҫע�����¼���
   1> ʹ��playready���֮ǰ���û�������΢������ȡ��PlayReady�����豸֤�顣
      û��ȡ��PlayReady�豸֤�����޷�ʹ��PlayReady����ġ�
	  
   2> Ĭ�ϵ�playready֤�����·��Ϊ/system/bin/prpd/����֤��ŵ���Ŀ¼��ʱ��ϵͳ���Զ���ȡ֤�飬
      ������Ҫ��������������ָ��֤��·����
	  ./sample_localplay ./xxxx.pyv --drm trans=1 mimetype=application/vnd.ms-playready DrmPath=./prpd/,LocalStoreFile=data.localstore
	  ע�⣺./prpd���һ��Ҫ�з���'/'��
	        ./xxxx.pyv��drm���ܵ���Ƶ�ļ���
						
   3> ��Ҫע��libHiplayreadyEngine.so�⡣��libHiplayreadyEngine.so�ŵ�/system/lib/drm/Ŀ¼��ʱ��HiPlayer���Զ�ע��ÿ��ļ���
      ������Ҫ���±���������ָ���ÿ��ļ��Ĵ��·����
	  ./sample_localplay ./xxxx.pyv --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so
	 
   4> ����ʹ�õĸ���playready����Ƶ�ļ���cms�ļ��ɴ�΢��Ĺٷ���վ��ȡ��
	  http://playready.directtaps.net/pr/public/rightstests.aspx?detail=thO/caiU10mTrzhLjSR06g==
	  
   5> ���õĲ��ԣ�
      ��ͨ���ԣ�
	     ./sample_localplay ./xxxx.pyv --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so
		 
	  domain���ԣ�����domain)����Ҫ����վ�����ؾ���domain֤�������
	      ./sample_localplay ./xxxx.pyv --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so --drm trans=100 mimetype=application/vnd.ms-playready InitiatorFile=JoinDomain.cms
	  domain���ԣ��뿪domain)����Ҫ����վ�����ؾ���domain֤�������
	      ./sample_localplay ./xxxx.pyv --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so --drm trans=100 mimetype=application/vnd.ms-playready InitiatorFile=LeaveDomain.cms
		  
	  envelope���ԣ�
	      ./sample_localplay ./envelope_test.asf --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so --drm trans=102 mimetype=application/vnd.ms-playready EnvelopFile=envelope_test.pye,\
		  EnvelopOutputFile=envelope_test.asf 
		 ���У�envelope_test.asf��envelope_test.pye�������ܺ����ɵ��ļ�
	   
	  metering���ԣ���Ҫ����վ�����ؾ���metering֤�������
	      metering�����������Ʋ��Ŵ�����
		  ./sample_localplay xxxx.asf --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so --drm trans=100 mimetype=application/vnd.ms-playready InitiatorFile=Metering.cms
		  �ڽ��ܸ��������ٴ��������������������������Metering����
	  


	  
����������չʾ��������IP���ݵĹ���
�����ʽ��sample_ipplay MultiAddr UdpPort
����˵����MultiAddr���鲥��ַ
          UdpPort: udp�˿�
����ο���./sample_ipplay 224.0.0.1 1234

ipplay 239.100.0.1 37000

ipplay 239.110.0.1 23456


����������չʾ����es�����Ź���
�����ʽ��sample_esplay vfile vtype afile atype
����˵����vfile: ��Ƶ�ļ�·��
          vtype: ��ƵЭ�����ͣ���ѡh264/mpeg2/mpeg4/avs/real8/real9/vc1ap/vc1smp5(WMV3)/vc1smp8/vp6/vp6f/vp6a/vp8/divx3/h263/sor
          afile: ��Ƶ�ļ�·�������ֻ����Ƶ
          atype����ƵЭ�����ͣ���ѡaac/mp3/dts/dra/mlp/pcm/ddp
����ο���sample_esplay ./h264.es h264 ./mp3.es mp3    
ע��������ֻ����Ƶ��vfile��vtype��Ҫ��Ϊnull�����ֻ����Ƶ��afile��atype��Ҫ��Ϊnull
	
	
