#include "Cup.h"
#include "core/EntryPoint.h"
#include "core/EditorLayer.h"

namespace Cup {

	class CupEditor : public Cup::CupEngine
	{
	public:
		CupEditor() {
			PushLayer(new EditorLayer(this));
		}
		~CupEditor() {

		}
	};

	Cup::CupEngine* Cup::CreateApplication() {
		return new CupEditor;
	}

}