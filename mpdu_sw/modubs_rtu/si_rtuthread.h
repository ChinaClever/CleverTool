�}S �   ��G�Im��u
��1�b!{�F>�fx�����=��V<]c0"-�Hv��E������kX6(�zt���R�8�h�b��k#P�Ӊ�^T���řX�/(�0t�mO����k�������E��)��4ȧ��)Sc38���¯��M��
�L��w3���T��v3A���2�X\���jz��v�q[n�X�ĸ��X䛏���T9���+���<'I:�,"ㆩ-��.(��\���N�3��`����_<d2�D���0�5n�H�xՊ
��oeYb�����@�t�Fa˹.��R�c����{���g?(��5|=��a�f�2��cH��ԡ�eq��m�ww�y;���rۏ�k�,s�ϲ�+��E�%�@q����k�`�e<��n�p0K�-��f��?[����R��W����i�+���g��E�$��΃)�d���7��_�,�����~����L�K3I
���N1�x���H�j���etCmd(int addr, int reg, ushort value, int msecs);
    void run();

signals:
    void cmdSig(QString);

public slots:

private:
    bool isRun;
    uchar *mBuf;
    SerialPort *mSerial;
    QMutex *mMutex;
    SI_RtuSent *mRtuSent;

    QList<sSetCmd> mList;
};

#endif // SI_RTUTHREAD_H
