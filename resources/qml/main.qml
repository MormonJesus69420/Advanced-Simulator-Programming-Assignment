import QtQuick 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import com.uit.GMlib2Qt 1.0

import "cameras"

Entity {
  id: root_entity

  property string scene_element_name: "scene_root_entity"
  property alias scenemodel: scene_root.scenemodel

  components: [ input_settings, render_settings]

  InputSettings { id: input_settings }

  RenderSettings {
    id: render_settings
    activeFrameGraph: renderer
    pickingSettings.pickMethod: PickingSettings.TrianglePicking
  }

  ForwardRenderer {
    id: renderer
    camera: projection_camera

    clearColor: "#c0c0c0"
  }

  ProjectionCamera {
    id: projection_camera
    position: Qt.vector3d(0.0, -20.0, 20.0)
    viewCenter: Qt.vector3d(0.0, 1.0, 0.0)
  }

//  OrbitCameraController{ camera: projection_camera }
  TraditionalGMlibCameraController{
    camera: projection_camera;
    scenemodel: root_entity.scenemodel }


  SceneRootEntity {
    id: scene_root
    objectName: scene_element_name
  }

} // END Entity (id:scene_root)
