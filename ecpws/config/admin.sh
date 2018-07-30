# /bin/sh
# ����Web Admin Service
# Ĭ��ʹ�����ģ�����ʹ������
#  ��־�� 2006.07.25
# ʹ�õķ�ʽΪ"admin.sh JA/zh_CN.GB2312"

echo "Start WebAdmin Servers"

WEB_ADMIN_MODULES="base ippbx services servers video"

export LC_ALL=

WEB_ADMIN_LOCALE=/etc/boa/admin.locale
if [ ! -f "$WEB_ADMIN_LOCALE" ] ; then
	echo " Locale Defination for Web Admin is missed, Default is \"Chinese\"" >& 2
else
	. $WEB_ADMIN_LOCALE
fi


if [ $ADMIN_LOCALE = "JA" ] ; then
	LC_ALL=ja
	echo "     web language: \"Japanese\""
else 
	if [ $ADMIN_LOCALE = "zh_CN.GB2312" ] ; then
		LC_ALL=zh_CN.GB2312
		echo "     Default web language: \"Chinese\""
	else
		LC_ALL=C
		echo "     Default web language: \"English\""
	fi
fi

echo "     Startup \"$WEB_ADMIN_MODULES\" ....."
mkdir -p /tmp/log/boa
for WEB_ADMIN in $WEB_ADMIN_MODULES; do
	name=$(eval "echo /tmp/log/boa/admin_${WEB_ADMIN}_error_log")
	echo "        Startup \"$WEB_ADMIN\" module ....."
	touch    $name
	echo 0 > $name
	name=$(eval "echo /tmp/log/boa/admin_${WEB_ADMIN}_access_log")
	touch    $name
	echo 0 > $name

	name=$(eval "echo /etc/boa/admin_$WEB_ADMIN.conf")
#	echo "configure file for $WEB_ADMIN is $name"
	/usr/bin/boa -f $name 
done
echo "     Startup All WebAdmin Servers Successfully!"
echo ""
echo ""
