// pages/choose_service/choose_service.js
const app = getApp()
Page({

  /**
   * 页面的初始数据
   */
  data: {
    deviceId: 0,
    leftPosition: 0,
    isFirst: true,
    title:''
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    let that = this
    this.setData({
      deviceId: options.deviceId,
      title:options.name
    })
    wx.setNavigationBarTitle({
      title: options.name,
    })
    wx.onBLEConnectionStateChange(function (res) {
      // 该方法回调中可以用于处理连接意外断开等异常情况
      console.log(`device ${res.deviceId} state has changed, connected: ${res.connected}`)
      if (!res.connected && res.deviceId == that.data.deviceId) {
        console.log('choose_service.js 连接已断开')
        that.closeBLEConnection(that.data.deviceId)
        that.setData({
          characteristicUUIDs: null,
          serviceUUIDs: null
        })
      }
    })
  },
  reload(res) {
    this.onReady()
  },
  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

    wx.showLoading({
      mask: true,
      title: 'connecting...',
    })
    let that = this
    let deviceId = this.data.deviceId
    wx.createBLEConnection({
      deviceId,
      success: (res) => {
        app.globalData.isConnect = true
        that.getBLEDeviceServices(deviceId)
      },
      fail: (res) => {
        console.log(res)
        setTimeout(() => {
          wx.hideLoading({
            complete: (res) => {
              that.setData({
                leftPosition: 0
              })
              that.showFail()
            },
          })
        }, 400)

      }
    })
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
    if (!this.data.isFirst) {
      if (!app.globalData.isConnect) {
        this.setData({
          characteristicUUIDs: null,
          serviceUUIDs: null
        })
      }

    }

    this.setData({
      isFirst: false
    })

  },
  onUnload: function () {
    wx.offBLEConnectionStateChange()
    this.closeBLEConnection(this.data.deviceId)
  },
  /**
   * 
   * 左边列表点击
   * @param {*} res 
   */
  leftClick(res) {
    let index = res.target.dataset.index
    if (index != this.data.leftPosition) {
      let lastPosition = this.data.leftPosition
      this.setData({
        leftPosition: index
      })
      wx.showLoading({
        mask: true,
        title: 'loadding...',
      })
      this.getBLEDeviceCharacteristics(this.data.deviceId, this.data.serviceUUIDs[index].uuid, lastPosition)

    }
  },
  /**
   * 发送信息页面
   * @param {*} res 
   */
  send(res) {
    let that = this
    let index = res.target.dataset.index
    let characteristicsUUID = that.data.characteristicUUIDs[index]
    let deviceId = that.data.deviceId
    let serviceUUID = that.data.serviceUUIDs[that.data.leftPosition]
    let obj = {
      characteristicsUUID: characteristicsUUID,
      deviceId: deviceId,
      serviceUUID: serviceUUID,
      deviceName:that.data.title,
      needClose:false
    }

    var data = JSON.stringify(obj);
    wx.navigateTo({
      url: '/pages/send/send?data=' + data,
    })

  },
  /**
   * 获取 serviceUUID
   * @param {*} deviceId 
   */
  getBLEDeviceServices(deviceId) {
    let that = this
    wx.getBLEDeviceServices({
      deviceId: deviceId,
      success: (data) => {
        // console.log('success', data)
        let serviceUUIDs = new Array()
        for (let i = 0; i < data.services.length; i++) {
          if (data.services[i].isPrimary) {
            serviceUUIDs.push(data.services[i])
          }
        }
        that.setData({
          serviceUUIDs: serviceUUIDs
        })
        that.getBLEDeviceCharacteristics(deviceId, serviceUUIDs[that.data.leftPosition].uuid, that.data.leftPosition)

      },
      fail: (res) => {
        console.log('fail', res)
        wx.hideLoading({
          complete: (res) => {
            setTimeout(() => {
              wx.hideLoading({
                complete: (res) => {
                  that.setData({
                    leftPosition: 0
                  })
                  wx.hideLoading({
                    complete: (res) => {
                      that.showFail()
                    },
                  })
                },
              })
            }, 400)
          },
        })
      }
    })

  },
  getBLEDeviceCharacteristics(deviceId, uuid, lastPosition) {
    let that = this
    wx.getBLEDeviceCharacteristics({
      deviceId: deviceId,
      serviceId: uuid,
      success(data) {
        console.log('getBLEDeviceCharacteristics', data)
        setTimeout(() => {
          wx.hideLoading({
            complete: (res) => {
              let characteristicUUIDs = new Array()
              for (let i = 0; i < data.characteristics.length; i++) {
                let item = data.characteristics[i]
                characteristicUUIDs.push(item)
              }
              that.setData({
                characteristicUUIDs: characteristicUUIDs
              })

            },
          })
        }, 400)
      },
      fail(res) {
        console.log(res)
        setTimeout(() => {
          wx.hideLoading({
            complete: (res) => {
              that.setData({
                leftPosition: lastPosition
              })
              that.showFail()
            },
          })
        }, 400)
      }
    })

  },
  showFail() {
    let that = this
    wx.showToast({
      title: '连接失败',
      icon: 'none',
      duration: 1000
    })
    that.closeBLEConnection(that.data.deviceId)
    that.setData({
      characteristicUUIDs: null,
      serviceUUIDs: null
    })
  },
  closeBLEConnection(deviceId) {
    wx.closeBLEConnection({
      deviceId,
      success(res) {
        console.log('closeBLEConnection success', res)
        wx.showToast({
          title: 'ble已断开',
          icon: 'none',
          duration: 1000
        })
      },
      fail(res) {
        console.log('closeBLEConnection fail', res)
      }
    })
  }

})