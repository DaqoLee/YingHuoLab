const formatTime = date => {
  const year = date.getFullYear()
  const month = date.getMonth() + 1
  const day = date.getDate()
  const hour = date.getHours()
  const minute = date.getMinutes()
  const second = date.getSeconds()

  return [year, month, day].map(formatNumber).join('/') + ' ' + [hour, minute, second].map(formatNumber).join(':')
}

const formatNumber = n => {
  n = n.toString()
  return n[1] ? n : '0' + n
}
const bytes2HexString = arrBytes => {
  var str = "";
  for (var i = 0; i < arrBytes.length; i++) {
    var tmp;
    var num = arrBytes[i];
    if (num < 0) {
      //此处填坑，当byte因为符合位导致数值为负时候，需要对数据进行处理
      tmp = (255 + num + 1).toString(16);
    } else {
      tmp = num.toString(16);
    }
    if (tmp.length == 1) {
      tmp = "0" + tmp;
    }
    str += tmp;
  }
  return str;
}
const hexString2Bytes = str => {
  var pos = 0;
  var len = str.length;
  if (len % 2 != 0) {
    return null;
  }
  len /= 2;
  var arrBytes = new Array();
  for (var i = 0; i < len; i++) {
    var s = str.substr(pos, 2);
    var v = parseInt(s, 16);
    arrBytes.push(v);
    pos += 2;
  }
  return arrBytes;
}

const hexToDEC = arr => {
  if (arr.length == 2) {
    //  (0x04<<8)|0xE2
    return (arr[0] << 8) | arr[1]
  }
  if (arr.length == 1) {
    return (0x00 << 8) | arr[0]
  }
  return 0
}

const abToHEX = buffer => {
  let hexArr = Array.prototype.map.call(
    new Uint8Array(buffer),
    function (bit) {
      return ('00' + bit.toString(16)).slice(-2)
    }
  )
  return hexArr.join('');
}

const hexCharCodeToStr = hexCharCodeStr => {
  var trimedStr = hexCharCodeStr.trim();
  var rawStr =
    trimedStr.substr(0, 2).toLowerCase() === "0x" ?
    trimedStr.substr(2) :
    trimedStr;
  var len = rawStr.length;
  if (len % 2 !== 0) {
    alert("Illegal Format ASCII Code!");
    return "";
  }
  var curCharCode;
  var resultStr = [];
  for (var i = 0; i < len; i = i + 2) {
    curCharCode = parseInt(rawStr.substr(i, 2), 16); // ASCII Code Value
    resultStr.push(String.fromCharCode(curCharCode));
  }
  return resultStr.join("");
}

const compressData = (params1, params2) => {
  if (params1 >= 255) {
    params1 = 255
  }
  let p1 = params1.toString(16)
  if (p1.length < 2) {
    p1 = '0' + p1
  } 

  let p2 = params2.toString(16)
  let len = p2.length

  for (let i = 0; i < 4 - len; i++) {
    p2 = '0' + p2
  }
  // console.log('p1',p1) 
  // console.log('p2',p2) 
  let cmd = 'EE' + p1.toLocaleUpperCase() + p2.toLocaleUpperCase() + 'FFFCFFFF'

  return cmd

}
const fomatNumber = (value,size)=>{
  var v = parseInt(value) //强转Int，毕竟有可能返回是String类型的数字
  return v.toFixed(size)
}

module.exports = {
  formatTime: formatTime,
  bytes2HexString: bytes2HexString,
  hexString2Bytes: hexString2Bytes,
  hexToDEC: hexToDEC,
  abToHEX: abToHEX,
  hexCharCodeToStr: hexCharCodeToStr,
  compressData: compressData,
  fomatNumber: fomatNumber
}