�}, �   ��"��[Gd��u
���b����?�>d��֡PU4�BF���O�z})��K_�L�|�N��,�}D
{@���L�9V>���j�1vb
[�Ȃ���|����ʁw9v�~��4
�.=&�	�0,���դ^ ����Nq9�ʵ�VA���� �.��Yt�¤����6�/�����Qj �fo����DE�,v'�֋q&#?���x��"���|~Bܬz���mQ:d?.%�@xӻ�r�Rg��?G��`�8��nm=��d�?�����bǰ�}r���ݨp��e�J��i��� u����L�g?(��5|=��a�f�2��cH��ԡ�eq��m�ww�y;���rۏ�k�,s�ϲ�+��E�%�@q����k�`�e<��n�p0K�-��f��?[����R��W����i�+���g��E�$��΃)�d���7��_�,�����~����L�K3I
���N1�x���H�j���on_clicked();
    void on_openRadio_clicked(bool checked);

    void on_closeRadio_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    SerialPortWid *mSerialPortWid;
    SI_RtuThread *mRtu;

    OutputWid *mWid[24];
};

#endif // MAINWINDOW_H
