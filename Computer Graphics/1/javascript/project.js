/* global THREE */

const frustumSize = 100;

var camera, frontCamera, topCamera, sideCamera;
var scene, renderer, clock, delta;
var articulatedObject, planet, ring, spaceship, objects = [];

let keys = {
    number4: false,
    q: false,
    w: false,
    a: false,
    s: false,
    z: false,
    x: false,
    c: false,
    d: false,
    arrowLeft: false,
    arrowUp: false,
    arrowRight: false,
    arrowDown: false,
    add: false,
    sub: false
  };


function createBox(color, width, height, depth) {
    'use strict';

    let geometry = new THREE.BoxGeometry(width, height, depth);
    let material = new THREE.MeshBasicMaterial({ color: color, wireframe: true });
    var box = new THREE.Mesh(geometry, material);

    scene.add(box);
    objects.push(box);
    return box;
}

function createCone(color, radius, height, radialSegments, heightSegments) {
    'use strict';

    let geometry = new THREE.ConeGeometry(radius, height, radialSegments, heightSegments);
    let material = new THREE.MeshBasicMaterial({ color: color, wireframe: true });
    let cone = new THREE.Mesh(geometry, material);

    scene.add(cone);
    objects.push(cone);
    return cone;
}

function createTorus(color, radius, tube, radialSegments, tubularSegments, arc) {
    'use strict';

    let geometry = new THREE.TorusGeometry(radius, tube, radialSegments, tubularSegments, arc);
    let material = new THREE.MeshBasicMaterial({ color: color, wireframe: true });
    let torus = new THREE.Mesh(geometry, material);

    scene.add(torus);
    objects.push(torus);
    return torus;
}

function createSphere(color, radius, widthSegments, heightSegments, phiStart, phiLenght, thetaStart) {
    'use strict';

    let geometry = new THREE.SphereGeometry(radius, widthSegments, heightSegments, phiStart, phiLenght, thetaStart);
    let material = new THREE.MeshBasicMaterial({ color: color, wireframe: true });
    let sphere = new THREE.Mesh(geometry, material);

    scene.add(sphere);
    objects.push(sphere);
    return sphere;
}

function createCylinder(color, radiusTop, radiusBottom, height, radialSegments) {
    'use strict';

    let geometry = new THREE.CylinderGeometry(radiusTop, radiusBottom, height, radialSegments);
    let material = new THREE.MeshBasicMaterial({ color: color, wireframe: true });
    let cylinder = new THREE.Mesh(geometry, material);

    scene.add(cylinder);
    objects.push(cylinder);
    return cylinder;
}

function createObjects() {
    'use strict';

    let object;

    object = createSphere(0xd40d5c, 5, 5, 5);
    object.rotation.set(0, Math.PI / 3, Math.PI / 3);
    object.position.set(-5, -10, 10);

    object = createTorus(0xffff00,  10, 3, 16, 6);
    object.rotation.set(3 * Math.PI / 17, Math.PI / 18, Math.PI / 2);
    object.position.set(-40, 27, -9);

    object = createTorus(0x62f47a, 6.384, 5, 2, 4);
    object.rotation.set(3 * Math.PI / 17, Math.PI / 18, Math.PI / 2);
    object.position.set(40, 27, -9);

    object = createTorus(0xf67904, 13, 1.5, 16, 20, 2.8);
    object.rotation.set(Math.PI, -Math.PI, Math.PI * 2);
    object.position.set(27, -27, -9);

    object = createBox(0xf462d9, 10, 10, 10);
    object.rotation.set(Math.PI, 2 * Math.PI / 6, Math.PI / 3);
    object.position.set(30, -17, 15);

    object = createBox(0x00ffbb, 2, 300, 2);
    object.rotation.set(0, Math.PI / 3, Math.PI / 3);

    object = createCylinder(0x7700ff, 2, 10, 300, 3);
    object.rotation.set(0, Math.PI / 3, - Math.PI / 3);
    object.position.set(-5, 0, 0);

    object = createCylinder(0xa4c12f, 7, 7, 25, 15);
    object.rotation.set(15, 25, 69);
    object.position.set(-20, -20, -20);

    object = createCone(0xff9f80, 6, 70, 4, 1)
    object.rotation.set(15,25,69);
    object.position.set(20,20,20);

    object = createCylinder(0x049ef4, 20, 20, 1, 3);
    object.rotation.set(Math.PI /4,  0, Math.PI /4);
    object.position.set(0,25, -35);

    object = createSphere(0xff0000, 15, 20, 16, 0, 2 * Math.PI, Math.PI / 2);
    object.rotation.set(Math.PI / 2, Math.PI / 3, - Math.PI / 6);
    object.position.set(50, -5, 30);

    object = createSphere(0x83291, 15, 20, 16, 0, 2 * Math.PI, Math.PI / 2);
    object.rotation.set(Math.PI / 2, Math.PI / 3, -7 * Math.PI / 6);
    object.position.set(52, -3, 34);
}

function createArticulatedObject(x, y, z) {
    'use strict';

    articulatedObject = new THREE.Object3D();
    articulatedObject.userData = { scale: 1 };

    let radius = 10;

    planet = createSphere(0xc73bd1, radius, 20, 20);

    ring = createTorus(0x00cc66, radius + 3, 1.5, 3, 20, 5.7);
    ring.rotation.set(3 * Math.PI / 5, Math.PI / 18, Math.PI / 2);   // values to look cool

    spaceship = createCylinder(0xffffff, 0, 2, 2.8, 3);
    spaceship.position.set(radius + 3, - radius / 3, 0);

    ring.add(spaceship);
    planet.add(ring);
    articulatedObject.add(planet);

    articulatedObject.position.set(x, y, z);

    scene.add(articulatedObject);
}

function createScene() {
    'use strict';

    scene = new THREE.Scene();

    createObjects();
    createArticulatedObject(-35, 0, 0);  // position x, y, z
}

function createCamera(x, y, z) {
    'use strict';

    let aspect = window.innerWidth / window.innerHeight;

    let camera = new THREE.OrthographicCamera(frustumSize * aspect / -2,
                                              frustumSize * aspect / 2,
                                              frustumSize / 2,
                                              frustumSize / -2,
                                              1,
                                              1000 );

                                              camera.position.x = x;
                                              camera.position.y = y;
                                              camera.position.z = z;
                                              camera.lookAt(scene.position);

    return camera;
}

function onKeyDown(e) {
    'use strict';

    switch (e.keyCode) {
        case 49:    // 1
             camera = "front";
             break;
        case 50:    // 2
            camera = "top";
            break;
        case 51:    // 3
            camera = "side";
            break;
        case 81:    // Q
        case 113:   // q
            keys.q = true;
            break;
        case 87:    // W
        case 119:   // w
            keys.w = true;
            break;
        case 65:    // A
        case 97:    // a
            keys.a = true;
            break;
        case 83:    // S
        case 115:   // s
            keys.s = true;
            break;
        case 90:    // Z
        case 122:   // z
            keys.z = true;
            break;
        case 88:    // X
        case 120:   // x
            keys.x = true;
            break;
        case 37:    // ArrowLeft
            keys.arrowLeft = true;
            break;
        case 38:    // ArrowUp
            keys.arrowUp = true;
            break;
        case 39:    // ArrowRight
            keys.arrowRight = true;
            break;
        case 40:    // ArrowDown
            keys.arrowDown = true;
            break;
        case 68:    // D
        case 100:   // d
            keys.d = true;
            break;
        case 67:    // C
        case 99:    // c
            keys.c = true;
            break;
        case 43:    // +
        case 107:   // +
        case 187:   // +
            keys.add = true;
            break;
        case 45:    // -
        case 109:   // -
        case 189:   // -
            keys.sub = true;
            break;
    }
}

function onKeyUp(e) {
    'use strict';

    switch (e.keyCode) {
        case 52:    // 4
            keys.number4 = true;
            break;
        case 81:    // Q
        case 113:   // q
            keys.q = false;
            break;
        case 87:    // W
        case 119:   // w
            keys.w = false;
            break;
        case 65:    // A
        case 97:    // a
            keys.a = false;
            break;
        case 83:    // S
        case 115:   // s
            keys.s = false;
            break;
        case 90:    // Z
        case 122:   // z
            keys.z = false;
            break;
        case 88:    // X
        case 120:   // x
            keys.x = false;
            break;
        case 37:    // ArrowLeft
            keys.arrowLeft = false;
            break;
        case 38:    // ArrowUp
            keys.arrowUp = false;
            break;
        case 39:    // ArrowRight
            keys.arrowRight = false;
            break;
        case 40:    // ArrowDown
            keys.arrowDown = false;
            break;
        case 68:    // D
        case 100:   // d
            keys.d = false;
            break;
        case 67:    // C
        case 99:    // c
            keys.c = false;
            break;
        case 43:    // +
        case 107:   // +
        case 187:   // +
            keys.add = false;
            break;
        case 45:    // -
        case 109:   // -
        case 189:   // -
            keys.sub = false;
            break;
    }
}

function resizeCamera(camera) {
    'use strict';

    let aspect = window.innerWidth / window.innerHeight;

    camera.left = frustumSize * aspect / -2;
    camera.right = frustumSize * aspect / 2;
    camera.top = frustumSize / 2;
    camera.bottom = frustumSize / -2;

    camera.updateProjectionMatrix();
}

function onWindowResize() {
    'use strict';

    resizeCamera(frontCamera);
    resizeCamera(topCamera);
    resizeCamera(sideCamera);

    renderer.setSize(window.innerWidth, window.innerHeight);
}

function init() {
    'use strict';

    renderer = new THREE.WebGLRenderer({ antialias: true });

    renderer.setSize(window.innerWidth, window.innerHeight);

    clock = new THREE.Clock();
    delta = 0;

    document.body.appendChild(renderer.domElement);

    createScene();
    frontCamera = createCamera(0, 0, 100);
    topCamera = createCamera(0, 100, 0);
    sideCamera = createCamera(100, 0, 0);

    camera = "front";

    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("keyup", onKeyUp);
    window.addEventListener("resize", onWindowResize);
}

function render(camera) {
    'use strict';

    renderer.render(scene, camera);
}

function animate() {
    'use strict';

    delta = clock.getDelta();

    if (keys.number4) {
        objects.forEach(object => {
            object.material.wireframe = !object.material.wireframe;
        });
        keys.number4 = false;
    }
    if (keys.q) {
        planet.rotateY(-0.5 * delta); 
    }
    if (keys.w) {
        planet.rotateY(0.5 * delta); 
    }
    if (keys.a) {
        ring.rotateZ(1.5 * delta); 
    }
    if (keys.s) {
        ring.rotateZ(-1.5 * delta); 
    }
    if (keys.z) {
        spaceship.rotateX(5 * delta);
        spaceship.rotateY(5 * delta);
    }
    if (keys.x) {
        spaceship.rotateX(-5 * delta);
        spaceship.rotateY(-5 * delta);
    }
    if (keys.arrowLeft) {
        articulatedObject.translateX(-20 * delta);
    }
    if (keys.arrowUp) {
        articulatedObject.translateY(20 * delta);
    }
    if (keys.arrowRight) {
        articulatedObject.translateX(20 * delta);
    }
    if (keys.arrowDown) {
        articulatedObject.translateY(-20 * delta);
    }
    if (keys.d) {
        articulatedObject.translateZ(-20 * delta);
    }
    if (keys.c) {
        articulatedObject.translateZ(20 * delta);
    }
    if (keys.add) {
        articulatedObject.userData.scale += 0.01;
        articulatedObject.scale.set(articulatedObject.userData.scale,
                         articulatedObject.userData.scale,
                         articulatedObject.userData.scale);
    }
    if (keys.sub) {
        articulatedObject.userData.scale -= 0.01;
        articulatedObject.scale.set(articulatedObject.userData.scale,
                         articulatedObject.userData.scale,
                         articulatedObject.userData.scale);
    }
    
    switch (camera) {
        case "front":
            render(frontCamera);
            break;
        case "top":
            render(topCamera);
            break;
        case "side":
            render(sideCamera);
            break;
    }
    
    requestAnimationFrame(animate);
}