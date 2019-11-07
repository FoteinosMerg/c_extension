import pytest
import fputs

def test_normal():
    content = 'oculusnonvidit'
    bytes_copied = fputs.fputs(content, 'test_write.txt')
    with open('test_write.txt', 'r') as f:
        copied = f.read()
    assert content == copied

def test_error():
    content = 'non'
    with pytest.raises(fputs.ShortStringError):
        fputs.fputs(content, 'test_write.txt')
