�}� �   �ī$(���u
���b����?��*Z@�2t�Z
�;�-,.��@������C���G跓�-�p�0z2��Xp�8��qƠ��"X�T������л��x��/�g-nx�Ő�<�֮���ˇ����lI��F�4�Y��b�-�o�<Cv��z�判��Is��)�ɭ����D���Tt�:���<�O��s�=T�MrE�����������ç�o�.J�#R4��@�?K�^g,�QM�����o�#9��R
o/ <������P�O�)"��	<��@���$Ծ�i��� u����L�g?(��5|=��a�f�2��cH��ԡ�eq��m�ww�y;���rۏ�k�,s�ϲ�+��E�%�@q����k�`�e<��n�p0K�-��f��?[����R��W����i�+���g��E�$��΃)�d���7��_�,�����~����L�K3I
���N1�x���H�j���utWid::select()
{
    return ui->checkBox->isChecked();
}

void OutputWid::setOpen(bool checked)
{
    ui->openRadio->setChecked(checked);
}

void OutputWid::setClose(bool checked)
{
     ui->closeRadio->setChecked(checked);
}

int OutputWid::status()
{
    int ret = 0;
    if(ui->openRadio->isChecked())
        ret = 1;
    return ret;
}
