�}� �   �AmC#R��u
��z�b����O�|8��1t7���_.7�Tt���Q����Q�J-o�l������v�E��,C�u�C�h!�_�m��O�A�R��uS>����4g췠�$k��S�~�Y	�`���Wg��%�<}��X�Q7!��d&X���S+�s{��!w�Jf�@���@��G��r������Pq��G�W�"G�z� ���3�y�f]�!��b�~MҢn�/���������<A�t���Gq�~�e\���2/��PP2E�Q�V2�vi{��n)cb��b]��h��TՍp��m�'���e�h���U��͊M"i���Ep���>�ۏ������l�W�����.�k?8, �&�\�c��v��聹=��`�e<��n�p0K�-��f��?[����R��W����i�+���g��E�$��΃)�d���7��_�,�����~����L�K3I
���N1�x���H�j���l�1�P���؈�[�͉wDIp��.�W�\/�k�29I@?��ۨ�E�&�"~���O�eMX�VW٫s�8۟���!L_�K7�1����GC���Y���JT��j�Y�njrĽ֢x%�۱~t@t���:y���6��Jj��w�㵧�<1�=��P�>=�n��:�r|sQb՝ ��90ҡ�������֓�$��&�]�~��Q��<��Ԅ-���i.Zӄt��2�>��5t����@ݑ�PP�@I���//DE$��!�@�R���fVdP;�@W:� �!�Y��쒱��|��G�2z��/ɕ\����!j��Og���:��5 ?����O��9��)��gP����N֕���Ŧ�I"���n>��a��S��QJ�E���	�J�L
`w4�$��	a���\s~v�����%��%6����������V��'����!�*K��� �Q��G?մ�'�o^����B��Q �W�r�N���l�vq������S�K�g,�C6�:l�!�J!�R���Q1I���7^gP@���O��3:oi�v7D��y)��h�}�S3��l�ss� ݽC�K�����j}�T�x�Ֆ������n�h]Y�!$�U��̀B�(ĢD+ų���/��5����.�"�>�;��u�������wAe&*�>����IO(dT ףgDDx� ?7{}����^��[�����:99�Hֱ�S��:o;�Nə�d���U�����zA����c��Cz�ҭ�O�lkgj1������_5,�5���	��Q�B˫	�j��a؅%2��V-1Wo��D��*���R,�K�;@� �"�HP�es,�R$J��E3�\�ȺY��mQ�6f�H�K��{S������Y�ַB�`�<?wFx��d��rh:��cѮ�ߌ/�ȿ��kBP�%��a� y�m����k���	�Ϙu�?=.9N���j
Z���mS�eo�H�s�v�.��W���jː��e֟>�N@D�u?o���D>�l7���PX��z+�� T����0O+(�J"26��	Ġ�;��:J��a�?�>	��W%Zb`�&{1���n�l�hO��WY(,�-�w�DT��wm]d�pW[�hv�t荃!n�M����!�t�lx��>�`5�QV`b��5/������VTn��HUN�z��]����d"�'&��NF��%A�`�ޭ��o[w����=4�έY�١����"=��ɊP6k��͠?;�?��H���[���=[�KrT���<��K2MO	#߻��1�f����2 G~�����#�ԃ�B�5��|��#Wb��YW���h�r�3���8c�ʇY
��\B�堾�,4�1qW���Efb��mU|��c4"�ϼ����:�JVl��#��r3���׾�����-;�幚S��Cަ��U��lm.���y�r�Fr��"PNr�7�\��<Qld�%.[�U�Ky�<O��񽱟�f����@Ӎ8����9k�|1��RҋDɲ��OarameterName, QString strParameter, QString strGroup)
{
    strParameterName = "/" + strGroup + "/" + strParameterName;
    pConfigIni->setValue(strParameterName, strParameter);
}

/**
 * 功 能：写入参数
 * 开发人员：Lzy     2016 - 七夕
 */
void sys_configFile_writeParam(QString name, QString value, QString strGroup)
{
    sys_configFile_open();
    sys_configFile_write(name, value, strGroup);
    sys_configFile_close();
}
