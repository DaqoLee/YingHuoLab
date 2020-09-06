// pages/send/send.js
const app = getApp()
var util = require('../../utils/util.js');
Page({
  /**
   * 页面的初始数据
   */
  data: {
    receivedData: '',
    lineBreak: false, // 换行
    isHexShow: false, // 16进制显示
    sendLineBreak: false,
    sendHex: false,
    sendMsg: 'YingHou',
    disabled:false,
    rgbAll: 'rgb(255,0,0)',//初始值
    rgb1: 'rgb(255,0,0)',//初始值
    rgb2: 'rgb(255,0,255)',//初始值
    rgb3: 'rgb(255,255,0)',//初始值
    rgb4: 'rgb(0,0,255)',//初始值
    rgb5: 'rgb(0,255,255)',//初始值
    rgb6: 'rgb(0,255,0)',//初始值
    pick: false,
    isGame: false,
    isPure:1,
    num:0
  },

   // 显示取色器
   toPick: function () {
    this.setData({
      pick: true
    })
  },

  setNum(res){
    this.setData({
      num: res.currentTarget.dataset.num
    })
    
  },
  //取色结果回调
  pickColor(e) {
   // let rgb = e.detail.color;
   var that=this;
   var val= e.detail.num;

   this.setData({
    rgbAll: e.detail.color
  })
   switch(this.data.num)
   {
      case 0:
        this.setData({
          rgbAll: e.detail.color
        })
      break;
      case 1:
        this.setData({
          rgb1: e.detail.color
        })
      break;
      case 2:
        this.setData({
          rgb2: e.detail.color
        })
      break;
      case 3:
        this.setData({
          rgb3: e.detail.color
        })
      break;
      case 4:
        this.setData({
          rgb4: e.detail.color
        })
      break;
      case 5:
        this.setData({
          rgb5: e.detail.color
        })
      break;
      case 6:
        this.setData({
          rgb6: e.detail.color
        })
      break;
   }

 
  //that.sendColor(rgb)
  },

  toDir(res){
    this.setData({
      isGame:  res.currentTarget.dataset.value
    })
  },

  toPure(res){
    this.setData({
      isPure:  res.currentTarget.dataset.value
    })
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    let d = JSON.parse(options.data)
    let properties = d.characteristicsUUID.properties
    this.setData({
      data: d,
      disabled:!properties.write,
    })
    // console.log(this.data.data)
    let that = this
    wx.setNavigationBarTitle({
      title: that.data.data.deviceName,
    })
    
    wx.onBLEConnectionStateChange(function (res) {
      // 该方法回调中可以用于处理连接意外断开等异常情况
      console.log(`device ${res.deviceId} state has changed, connected: ${res.connected}`)
      if (!res.connected && res.deviceId == that.data.data.deviceId) {
        console.log('连接已断开')
        that.closeBLEConnection(that.data.data.deviceId)
        app.globalData.isConnect = false
        wx.showModal({
          title: '提示',
          content: '连接已断开',
          showCancel: false,
          success(res) {
            if (res.confirm) {
              console.log('用户点击确定')

              wx.navigateBack({
                delta: 1
              })
            }
          }
        })

      }
    })

  },
  /**
   * 文本框输入
   * @param {*} res 
   */
  sendInput(res) {
    // console.log(res)
    let value = res.detail.value
    this.setData({
      sendMsg: value
    })

  },


  sendTest(res) {
    // console.log(res)currentTarget.dataset.index
    let value = res.currentTarget.dataset.value
    this.setData({
      sendMsg: value,
     // sendLineBreak: true, // 换行
      sendHex: true // 16进制显示
    })
  },


  // sendColor(res) {
  //   // console.log(res)currentTarget.dataset.index
  //   let value = res.currentTarget.dataset.value
  //   let arr= value.match(/\d+/g)
  //   // alert(arr[0].toString(16));  
  //   this.setData({
  //     sendMsg: "AA"+"01"+"00"+this.PrefixZero(parseInt( arr[0] ).toString(16).toUpperCase(),2)+this.PrefixZero(parseInt( arr[2] ).toString(16).toUpperCase(),2)+this.PrefixZero(parseInt( arr[1] ).toString(16).toUpperCase(),2)+"AA",
  //    // sendLineBreak: true, // 换行
  //     sendHex: true // 16进制显示
  //   })
  // },
/**
* 自定义函数名：PrefixZero
* @param num： 被操作数
* @param n： 固定的总位数
*/
 PrefixZero(num, n) {
  return (Array(n).join(0) + num).slice(-n);
},

sendLumin(e) {
  // console.log(res)currentTarget.dataset.index
  let value = e.detail.value
  this.setData({
    sendMsg: "FF"+this.PrefixZero(parseInt(value).toString(16).toUpperCase(),2)+"FF",
    sendHex: true // 16进制显示
  })
},
  /**
   * send菜单变化
   * @param {*} res 
   */
  sendMenuChange(res) {
    // console.log(res)
    const values = res.detail.value
    let isHex = false
    let lineBreak = false
    for (let i = 0; i < values.length; i++) {
      if (values[i] == 'lineBreak') {
        lineBreak = true
      }
      if (values[i] == 'isHexShow') {
        isHex = true
      }
    }
    this.setData({
      sendLineBreak: lineBreak, // 换行
      sendHex: isHex // 16进制显示
    })
    // console.log(this.data.sendHex)
    // console.log(this.data.sendLineBreak)

  },
  /**
   * 接受信息菜单变化
   * @param {*} res 
   */
  receivedMenuChange(res) {
    // console.log(res)
    const values = res.detail.value
    let isHex = false
    let lineBreak = false
    for (let i = 0; i < values.length; i++) {
      if (values[i] == 'lineBreak') {
        lineBreak = true
      }
      if (values[i] == 'isHexShow') {
        isHex = true
      }
    }
    this.setData({
      lineBreak: lineBreak, // 换行
      isHexShow: isHex // 16进制显示
    })
    // console.log(this.data.lineBreak)
    // console.log(this.data.isHexShow)
  },
  clearReceived(res) {
    this.setData({
      receivedData: ''
    })
  },
  /**
   * 发送 消息
   * @param {*} res 
   */
sendColor(res) {
    let that = this
    let deviceId = that.data.data.deviceId
    let serviceId = that.data.data.serviceUUID.uuid
    let characteristicId = that.data.data.characteristicsUUID.uuid

    let value = res.currentTarget.dataset.value
    let arr= value.match(/\d+/g)
    let str1=this.PrefixZero(parseInt( this.data.num ).toString(16).toUpperCase(),2)
    let str2=this.PrefixZero(parseInt(this.data.isPure ).toString(16).toUpperCase(),2)
    this.setData({
    sendMsg: "AA"+str2+str1+this.PrefixZero(parseInt( arr[0] ).toString(16).toUpperCase(),2)+this.PrefixZero(parseInt( arr[2] ).toString(16).toUpperCase(),2)+this.PrefixZero(parseInt( arr[1] ).toString(16).toUpperCase(),2)+"AA",
      sendHex: true // 16进制显示
    })

    let senddata = that.data.sendMsg

    if(that.data.sendHex){
      // senddata = that.strToHexCharCode(senddata)
      senddata = senddata.toLocaleUpperCase()
      console.log('senddata',senddata)

      try{
        var typedArray = new Uint8Array(senddata.match(/[\da-f]{2}/gi).map(function (h) {
          return parseInt(h, 16)
        }))
     
    
        wx.writeBLECharacteristicValue({
          // 这里的 deviceId 需要在 getBluetoothDevices 或 onBluetoothDeviceFound 接口中获取
          deviceId,
          // 这里的 serviceId 需要在 getBLEDeviceServices 接口中获取
          serviceId,
          // 这里的 characteristicId 需要在 getBLEDeviceCharacteristics 接口中获取
          characteristicId,
          // 这里的value是ArrayBuffer类型
          value: typedArray.buffer,
          // success(res) {
          //   console.log('writeBLECharacteristicValue success', res.errMsg)
          //   wx.showToast({
          //     title: '已发送',
          //     icon: 'none',
          //     duration: 1000
          //   })
          // },
          // fail(res) {
          //   wx.showToast({
          //     title: '发送失败',
          //     icon: 'none',
          //     duration: 1000
          //   })
          // }
        })
      }catch(error){
        wx.showToast({
          title: '请输入16进制字符串:EE',
          icon: 'none',
          duration: 1000
        })

      }
      return

    }


    if (that.data.sendLineBreak) {
      senddata += '\r\n'
    }

    let buffer = new ArrayBuffer(senddata.length)
    let dataView = new DataView(buffer)
    for (var i = 0; i < senddata.length; i++) {
      dataView.setUint8(i, senddata.charAt(i).charCodeAt())
    }

    wx.writeBLECharacteristicValue({
      // 这里的 deviceId 需要在 getBluetoothDevices 或 onBluetoothDeviceFound 接口中获取
      deviceId,
      // 这里的 serviceId 需要在 getBLEDeviceServices 接口中获取
      serviceId,
      // 这里的 characteristicId 需要在 getBLEDeviceCharacteristics 接口中获取
      characteristicId,
      // 这里的value是ArrayBuffer类型
      value: buffer,
      // success(res) {
      //   console.log('writeBLECharacteristicValue success', res.errMsg)
      //   wx.showToast({
      //     title: '已发送',
      //     icon: 'none',
      //     duration: 1000
      //   })
      // },
      // fail(res) {
      //   wx.showToast({
      //     title: '发送失败',
      //     icon: 'none',
      //     duration: 1000
      //   })
      // }
    })
  },



  /**
   * 发送 消息
   * @param {*} res 
   */
  send(res) {
    let that = this
    let deviceId = that.data.data.deviceId
    let serviceId = that.data.data.serviceUUID.uuid
    let characteristicId = that.data.data.characteristicsUUID.uuid

    let senddata = that.data.sendMsg

    if(that.data.sendHex){
      // senddata = that.strToHexCharCode(senddata)
      senddata = senddata.toLocaleUpperCase()
      console.log('senddata',senddata)

      try{
        var typedArray = new Uint8Array(senddata.match(/[\da-f]{2}/gi).map(function (h) {
          return parseInt(h, 16)
        }))
     
    
        wx.writeBLECharacteristicValue({
          // 这里的 deviceId 需要在 getBluetoothDevices 或 onBluetoothDeviceFound 接口中获取
          deviceId,
          // 这里的 serviceId 需要在 getBLEDeviceServices 接口中获取
          serviceId,
          // 这里的 characteristicId 需要在 getBLEDeviceCharacteristics 接口中获取
          characteristicId,
          // 这里的value是ArrayBuffer类型
          value: typedArray.buffer,
          // success(res) {
          //   console.log('writeBLECharacteristicValue success', res.errMsg)
          //   wx.showToast({
          //     title: '已发送',
          //     icon: 'none',
          //     duration: 1000
          //   })
          // },
          // fail(res) {
          //   wx.showToast({
          //     title: '发送失败',
          //     icon: 'none',
          //     duration: 1000
          //   })
          // }
        })
      }catch(error){
        wx.showToast({
          title: '请输入16进制字符串:EE',
          icon: 'none',
          duration: 1000
        })

      }
      return

    }


    if (that.data.sendLineBreak) {
      senddata += '\r\n'
    }

    let buffer = new ArrayBuffer(senddata.length)
    let dataView = new DataView(buffer)
    for (var i = 0; i < senddata.length; i++) {
      dataView.setUint8(i, senddata.charAt(i).charCodeAt())
    }

    wx.writeBLECharacteristicValue({
      // 这里的 deviceId 需要在 getBluetoothDevices 或 onBluetoothDeviceFound 接口中获取
      deviceId,
      // 这里的 serviceId 需要在 getBLEDeviceServices 接口中获取
      serviceId,
      // 这里的 characteristicId 需要在 getBLEDeviceCharacteristics 接口中获取
      characteristicId,
      // 这里的value是ArrayBuffer类型
      value: buffer,
      // success(res) {
      //   console.log('writeBLECharacteristicValue success', res.errMsg)
      //   wx.showToast({
      //     title: '已发送',
      //     icon: 'none',
      //     duration: 1000
      //   })
      // },
      // fail(res) {
      //   wx.showToast({
      //     title: '发送失败',
      //     icon: 'none',
      //     duration: 1000
      //   })
      // }
    })
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

    let that = this
    let deviceId = that.data.data.deviceId
    let serviceId = that.data.data.serviceUUID.uuid
    let characteristicId = that.data.data.characteristicsUUID.uuid
    // 必须在这里的回调才能获取
    wx.onBLECharacteristicValueChange(function (res) {
      // console.log('characteristic value comed:', characteristic)
      let hexStr = that.ab2hex(res.value)

      // console.log('hex',hexStr)
      // console.log('str',that.hexCharCodeToStr(hexStr))
      if (!that.data.isHexShow) {
        hexStr = that.hexCharCodeToStr(hexStr)
      }

      let currentData = that.data.receivedData += hexStr
      if (that.data.lineBreak) {
        currentData += '\n'
      }
      that.setData({
        receivedData: currentData
      })


    })

    let properties = that.data.data.characteristicsUUID.properties

    if (properties.indicate || properties.notify) {

      wx.notifyBLECharacteristicValueChange({
        state: false, // 启用 notify 功能
        // 这里的 deviceId 需要已经通过 createBLEConnection 与对应设备建立链接
        deviceId,
        // 这里的 serviceId 需要在 getBLEDeviceServices 接口中获取
        serviceId,
        // 这里的 characteristicId 需要在 getBLEDeviceCharacteristics 接口中获取
        characteristicId,
        success(res) {
          console.log('notifyBLECharacteristicValueChange success', res.errMsg)

        }
      })
    }
  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {
    wx.offBLECharacteristicValueChange() // 取消监听低功耗蓝牙设备的特征值变化事件
    wx.offBLECharacteristicValueChange() // 取消监听低功耗蓝牙设备的特征值变化事件
  },
  onUnload(){
    if(this.data.data.needClose){
      this.closeBLEConnection(this.data.data.deviceId)

    }

  },
  closeBLEConnection(deviceId) {
    wx.closeBLEConnection({
      deviceId,
      success(res) {
        console.log('closeBLEConnection success', res)
        wx.showToast({
          title: 'BLE已断开',
          icon: 'none',
          duration: 1000
        })
      },
      fail(res) {
        console.log('closeBLEConnection fail', res)
      }
    })
  },

  ab2hex(buffer) {
    let hexArr = Array.prototype.map.call(
      new Uint8Array(buffer),
      function (bit) {
        return ('00' + bit.toString(16)).slice(-2)
      }
    )
    return hexArr.join('');
  },
  hexCharCodeToStr(hexCharCodeStr) {
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
  },
  strToHexCharCode(str) {
    if (str === "")
      return "";
    var hexCharCode = [];
   // hexCharCode.push("0x");
    for (var i = 0; i < str.length; i++) {
      hexCharCode.push((str.charCodeAt(i)).toString(16));
    }
    return hexCharCode.join("");
  }
})