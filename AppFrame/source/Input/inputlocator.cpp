#include "pch.h"
#include "inputlocator.h"


// ゲーム起動時から終了時まで、絶対にエラーを起こさないようする
std::shared_ptr<InputDevice> InputLocator::s_CurrentDevice = 
 std::shared_ptr<InputDevice>
	(
		&InputLocator::s_NullDevice,// ダミーを置いておく
		[](InputDevice*) {}// このデータが不要になっても、絶対にメモリから消去しないように
	);

NullInputDevice InputLocator::s_NullDevice;

void InputLocator::Provide(const std::shared_ptr<InputDevice>& device)
{
	if(device == nullptr)
	{
		s_CurrentDevice = std::shared_ptr<InputDevice>(&s_NullDevice, [](InputDevice*) {});// デバイスがnullptrの場合はNullオブジェクトを提供する
	}
	else
	{
		s_CurrentDevice = device;// デバイスが提供された場合はそのデバイスを提供する
	}
}

InputDevice& InputLocator::Get()
{
	return *s_CurrentDevice;// 現在のデバイスを返す
}