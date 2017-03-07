--[=======[
-------- -------- -------- --------
             sock����
-------- -------- -------- --------
]=======]

--[=======[
��
    Private:  UDP;
        --UDP����ʧ�ܽ��׳��쳣��ͬʱ!!�ͷŶ���!!

    UDP       udp_new         (
                              [
                              string|uint         ip    = "0.0.0.0",
                              string|uint         port  = "0",
                              string|uint         bind_port = "0"
                              ]
                              );                                    [-0|1|2|3, +1, c|v]
        --��ip == "0.0.0.0"��port != "0"ʱ����ָ���˿�
        --��ip != "0.0.0.0"ʱ��Ĭ������ָ��IP����ʱ������bind_portΪ�󶨶˿�
        
    string ip, string port, uint ip, uint port
              UDP:getsockname ( );                                          [-0, +1, c]

    string ip, string port, uint ip, uint port
              UDP:getpeername ( );                                          [-0, +1, c]

    stirng    UDP:type        ( );     --����"UDP"                          [-0, +1, c]
    
    void      UDP:close       ( );                                          [-0, +0, c]

    void      UDP:__gc        ( );                                          [-0, +0, c]

    string    UDP:__tostring  ( );                                          [-0, +1, c]
        --����UDP{server/client}:########    local_ip:port >> link_ip:port
        
    UDP       UDP:settimeout  ( int timeout );                              [-1, +1, v]
        --������ʱ�������

    UDP       UDP:broadcast   ( bool set );                                 [-1, +1, v]
        --�㲥����

    UDP       UDP:send        (
                              string              data
                              [,
                              string|uint         ip    = "0.0.0.0",
                              string|uint         port  = "0"
                              ]
                              );                                      [-1|2|3, +1, c|v]
        --�����ṩip��portʱ��Ĭ�����ӳ�ʼ��ʱָ����IP
        
    string data, string ip, string port, uint ip, uint port
              UDP:recv        ( [ int size = 0x800 ] );               [-0|1, +2|5, c|v]
        --�����ṩsizeʱ��Ĭ���ṩ0x800�Ľ��ջ�����
        --��ʱ����nil, "timeout"
        --���ջ��������㷵��nil, "msgsize"
]=======]


--[=======[
��
    Private:  TCP;
        --TCP����ʧ�ܽ��׳��쳣��ͬʱ!!�ͷŶ���!!

    TCP       tcp_new         (
                              string|uint         ip    = "0.0.0.0"
                              string|uint         port  = "0",
                              string|uint         bind_port = "0",
                              bool                NonBlockConnect = false
                              );                                          [-4, +1, c|v]
        
    string ip, string port, uint ip, uint port
              TCP:getsockname ( );                                          [-0, +1, c]

    string ip, string port, uint ip, uint port
              TCP:getpeername ( );                                          [-0, +1, c]

    stirng    TCP:type        ( );     --����"TCP"                          [-0, +1, c]
    
    void      TCP:close       ( );                                          [-0, +0, c]

    void      TCP:__gc        ( );                                          [-0, +0, c]

    string    TCP:__tostring  ( );                                          [-0, +1, c]
        --����TCP{server/client}:########    local_ip:port >> link_ip:port
        
    TCP       TCP:settimeout  ( int timeout );                              [-1, +1, v]
        --������ʱ������ơ�Server��֧��

    TCP       TCP:broadcast   ( bool set );                                 [-1, +1, v]
        --�㲥���á�Server��֧��

    TCP       TCP:send        ( string data );                            [-1, +1, c|v]
        --�������ݡ�Server��֧��
        
    string    TCP:recv        ( [ int size = 0x800 ] );               [-0|1, +1|2, c|v]
        --�����ṩsizeʱ��Ĭ���ṩ0x800�Ľ��ջ�����
        --��ʱ����nil, "timeout"��Server��֧��
    TCP       TCP:accept      ( [ int timeout == -1 ]);                 [-0|1, +1|2, v]
        --Client��֧��
        --�����ṩtimeoutʱ��Ĭ�ϳ�ʱֵ-1��������ֱ�����ӷ���
        --���ṩtimeout(�����)ʱ������ָ��ʱ�䣬ֱ�����ӷ�����ʱ����
        --���ӷ���ʱ�����������ӵ�TCP����
        --��ʱ����nil, "timeout"
    bool      TCP:check       ( );                                          [-0, +1, c]
        --���NonBlockConnect��TCP�Ƿ����ӳɹ����ɹ����Զ�����������
]=======]