import Qt3D.Core 2.0
import Qt3D.Render 2.0

Camera {
    id: root

    projectionType: CameraLens.OrthographicProjection
    nearPlane : 0.1
    farPlane : 1000.0

    position: Qt.vector3d(0.0, -20.0, 20.0)
    viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
    upVector: Qt.vector3d(0.0, 0.0, 1.0)
}
