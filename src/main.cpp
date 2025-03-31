#include <Geode/Geode.hpp>

using namespace geode::prelude;

#define public_cast(value, member) [](auto* v) { \
    class FriendClass__; \
    using T = std::remove_pointer<decltype(v)>::type; \
    class FriendeeClass__: public T { \
    protected: \
        friend FriendClass__; \
    }; \
    class FriendClass__ { \
    public: \
        auto& get(FriendeeClass__* v) { return v->member; } \
    } c; \
    return c.get(reinterpret_cast<FriendeeClass__*>(v)); \
}(value)

std::vector<std::string> g_Exclusions = {
	"thesillydoggo.qolmod/QOLModButton",
	"dankmeme.globed2/notification-panel",
	"eclipse.eclipse-menu/floating-button"
};

class SceneHandler : public CCNode {
public:
	static SceneHandler* create() {
		auto ret = new SceneHandler();
		ret->autorelease();
		return ret;
	}

	CCScene* m_currentScene = nullptr;

	void checkForChildrenChange(float dt) {
		if (m_bReorderChildDirty) {
			int zOrder = 0;

			for (CCNode* child : CCArrayExt<CCNode*>(getChildren())) {
				if (std::find(g_Exclusions.begin(), g_Exclusions.end(), child->getID()) != g_Exclusions.end()) continue;
				child->setZOrder(zOrder);
				zOrder += 1;
			}
		}
	}

	void update(float dt) {
		auto scene = CCDirector::sharedDirector()->getRunningScene();
		if (CCTransitionScene* trans = typeinfo_cast<CCTransitionScene*>(scene)) {
			scene = public_cast(trans, m_pInScene);
		}
		if (scene != m_currentScene) {
			m_currentScene = scene;
			m_currentScene->schedule(schedule_selector(SceneHandler::checkForChildrenChange));
		}
	}
};
	
$execute {
	Loader::get()->queueInMainThread([]{
		CCScheduler::get()->scheduleUpdateForTarget(SceneHandler::create(), INT_MAX, false);
	});
}