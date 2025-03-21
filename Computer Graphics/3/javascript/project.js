/* global THREE */

const viewSize = 200;

const pauseTexture = new THREE.TextureLoader().load('textures/pause.jpg')
  
var renderer, scene, clock, delta;
var activeCamera, perspectiveCamera, ortographicCamera;
var directionalLight, spotLight1, spotLight2, spotLight3;
var step1, step2, step3;
var paused, pauseScreen;
var currentMaterial = "phong", basic = false;
var objects = [];

var keys = {
    q: false,
    w: false,
    e: false,
    r: false,
    t: false,
    y: false,
    a: false,
    s: false,
    d: false,
    z: false,
    x: false,
    c: false,
    reset: false
};

function createBox(color, width, height, depth) {
    'use strict';

    let geometry = new THREE.BoxGeometry(width, height, depth);
    let material = new THREE.MeshPhongMaterial({ color: color });
    var box = new THREE.Mesh(geometry, material);
    box.receiveShadow = true;

    return box;
}

function createCone(color, radius, height, radialSegments, heightSegments) {
    'use strict';

    let geometry = new THREE.ConeGeometry(radius, height, radialSegments, heightSegments);
    let material = new THREE.MeshBasicMaterial({ color: color });
    let cone = new THREE.Mesh(geometry, material);

    return cone;
}

function createPlane(color, width, height, widthSegments, heightSegments) {
    'use strict';

    let geometry = new THREE.PlaneGeometry(width, height, widthSegments, heightSegments);
    let material = new THREE.MeshPhongMaterial({ color: color, side: THREE.DoubleSide });
    let plane = new THREE.Mesh(geometry, material);
    plane.receiveShadow = true;

    return plane;
}

function createSphere(color, radius, widthSegments, heightSegments, phiLenght, thetaLength) {
    'use strict';

    let geometry = new THREE.SphereGeometry(radius, widthSegments, heightSegments, 0, phiLenght, 0, thetaLength);
    let material = new THREE.MeshBasicMaterial({ color: color });
    let sphere = new THREE.Mesh(geometry, material);

    return sphere;
}

function createFloor(color) {
    'use strict';

    let floor = createPlane(color, 1000, 1000);
    floor.position.set(0, -50, 0);
    floor.rotation.set(Math.PI/2, 0, 0);

    objects.push(floor);
    scene.add(floor);
}

function createPodium(color, width, height, depth) {
    'use strict';

    let margin = depth / 10;

    let podium = new THREE.Group();

    let firstStep = createBox(color, width + margin*4, height/2, depth + margin*4);
    
    firstStep.position.set(0, height/4, 0);
    podium.add(firstStep);
    let secondStep = createBox(color, width + margin*2, height/2, depth + margin*2); 
    secondStep.position.set(0, height*3/4, 0);
    podium.add(secondStep);

    podium.position.set(0, -50, 0);

    objects.push(podium);
    scene.add(podium);
}

function createDirectionalLight(x, y, z) {
    'use strict';

    let light = new THREE.DirectionalLight(0xffffff, 0.5);
    light.castShadow = true;
    light.shadowCameraVisible = true;
    light.position.set(x, y, z);

    scene.add(light);
    return light;
}

function createSpotLight(x, y, z, target) {
    'use strict';

    let spotLight = new THREE.Group();

    let light = new THREE.SpotLight(0xffffff, 1, 300, 0.55, 0.1, 2, 1);
    light.castShadow = true;
    light.shadowCameraVisible = true;
    light.target = target;

    let cone = createCone(0x555555, 3, 4, 10, 10);
    cone.position.set(0, 2, 0);

    let sphere = createSphere(0xfcfbde, 2, 10, 10, Math.PI*2, Math.PI/2);
    sphere.position.set(0, 1, 0);
    sphere.rotation.set(Math.PI, 0, 0);
    
    spotLight.add(light);
    spotLight.add(cone);
    spotLight.add(sphere);

    spotLight.position.set(x, y, z);

    scene.add(spotLight);
    return spotLight;
}

function toggleLight(light) {
    'use strict';

    const on = new THREE.Color(0xc7c7c7);
    const off = new THREE.Color(0xfcfbde);

    if (light instanceof THREE.DirectionalLight) {
        if (light.intensity > 0) {
            light.intensity = 0;
        } else {
            light.intensity = 0.5;
        }
    } else {
        if (light.children[0].intensity > 0) {
            light.children[0].intensity = 0;
            light.children[2].material.color = on;
        } else {
            light.children[0].intensity = 1;
            light.children[2].material.color = off;
        }
    }
    
}

function createScene() {
    'use strict';

    const distance = 100;     // distance between steps

    scene = new THREE.Scene();
    scene.background = new THREE.Color(0xc999a5);

    createFloor(0xd18017);
    
    createPodium(0x550000, distance*3, distance/5, distance);

    step1 = createStep1(-distance, 0, 0);
    step2 = createStep2(0, 0, 0);
    step3 = createStep3(distance, -5, 0);

    directionalLight = createDirectionalLight(0, 50, 100);
    spotLight1 = createSpotLight(-distance, 50, 0, step1);
    spotLight2 = createSpotLight(0, 50, 0, step2);
    spotLight3 = createSpotLight(distance, 50, 0, step3);

    createPauseScreen();
}

function createPauseScreen() {
    'use strict';

    let geometry = new THREE.PlaneGeometry(80, 60, 1, 1);
    let material = new THREE.MeshBasicMaterial({ map: pauseTexture });
    pauseScreen = new THREE.Mesh(geometry, material);

    scene.add(pauseScreen);
}

function createCamera(type, x, y, z) {
    'use strict';

    let camera;

    let aspect = window.innerWidth / window.innerHeight;
    
    switch (type) {
        case "perspective":
            camera = new THREE.PerspectiveCamera(70, aspect, 1, 1000);
            break;
        case "ortographic":
            camera = new THREE.OrthographicCamera(viewSize * aspect / -2,
                                                  viewSize * aspect / 2,
                                                  viewSize / 2,
                                                  viewSize / -2,
                                                  1,
                                                  1000);
            break;
    }

    camera.position.set(x, y, z);

    return camera;
}

function resizeCamera(camera, type) {
    'use strict';

    let aspect = window.innerWidth / window.innerHeight;

    switch (type) {
        case "perspective":
            camera.aspect = aspect;
            break;
        case "ortographic":
            camera.left = viewSize * aspect / -2;
            camera.right = viewSize * aspect / 2;
            camera.top = viewSize / 2;
            camera.bottom = viewSize / -2;
            break;
    }

    camera.updateProjectionMatrix();
}

function onWindowResize() {
    'use strict';

    resizeCamera(perspectiveCamera, "perspective");
    resizeCamera(ortographicCamera, "ortographic");

    renderer.setSize(window.innerWidth, window.innerHeight);
}

function onKeyDown(e) {
    'use strict';

    if (e.keyCode == 49) {  // 1
        activeCamera = 1;
    } else if (e.keyCode == 50) {   // 2
        activeCamera = 2;
    }
    if (!paused) {
        switch (e.keyCode) {
            case 81:    // Q
            case 113:
                keys.q = true;
                break;
            case 87:    // W
            case 119:
                keys.w = true;
                break;
            case 69:    // E
            case 101:
                keys.e = true;
                break;
            case 82:    // R
            case 114:
                keys.r = true;
                break;
            case 84:    // T
            case 116:
                keys.t = true;
                break;
            case 89:    // Y
            case 121:
                keys.y = true;
                break;
        }
    }
        
}

function onKeyUp(e) {
    'use strict';

    switch (e.keyCode) {
        case 81:    // Q
        case 113:
            keys.q = false;
            break;
        case 87:    // W
        case 119:
            keys.w = false;
            break;
        case 69:    // E
        case 101:
            keys.e = false;
            break;
        case 82:    // R
        case 114:
            keys.r = false;
            break;
        case 84:    // T
        case 116:
            keys.t = false;
            break;
        case 89:    // Y
        case 121:
            keys.y = false;
            break;
        case 65:    // A
        case 97:
            keys.a = true;
            break;
        case 83:    // S
        case 115:
            keys.s = true;
            break;
        case 68:    // D
        case 100:
            keys.d = true;
            break;
        case 90:    // Z
        case 122:
            keys.z = true;
            break;
        case 88:    // X
        case 120:
            keys.x = true;
            break;
        case 67:    // C
        case 99:
            keys.c = true;
            break;
        case 32:    // P
            paused = !paused;
            break;
    }
    if (paused) {
        if (e.keyCode == 51) {  // 3
            keys.reset = true;
        }
    }
}

function initializeRenderer(){
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(window.innerWidth, window.innerHeight);
    renderer.shadowMap.enabled = true;
    renderer.shadowMap.type = THREE.PCFSoftShadowMap;
    renderer.xr.enabled = true;  
    document.body.appendChild(renderer.domElement);
}

function init() {
    'use strict';

    initializeRenderer();

    clock = new THREE.Clock();
    delta = 0;

    createScene();
    paused = false;

    perspectiveCamera = createCamera("perspective", 0, 50, 150);
    perspectiveCamera.lookAt(scene.position);
    ortographicCamera = createCamera("ortographic", 0, 0, 100);
    ortographicCamera.lookAt(scene.position);
    activeCamera = 1;

    window.addEventListener("resize", onWindowResize);
    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("keyup", onKeyUp);

    document.body.appendChild(VRButton.createButton(renderer));

    renderer.setAnimationLoop(animate); 
}

function animate() {
    'use strict';

    delta = clock.getDelta();

    if (paused) {
        switch (activeCamera) {
            case 1:
                pauseScreen.position.set(0, 12, 36);
                pauseScreen.rotation.set(-0.32175, 0, 0);
                break;
            case 2:
                pauseScreen.position.set(0, 0, 90);
                pauseScreen.rotation.set(0, 0, 0);
                break;
        }
        pauseScreen.visible = true;
    } else {
        pauseScreen.visible = false;
    }

    if (keys.q) {
        step1.rotateY(0.5 * delta);
    }
    if (keys.w) {
        step1.rotateY(-0.5 * delta);
    }
    if (keys.e) {
        step2.rotateY(0.5 * delta);
    }
    if (keys.r) {
        step2.rotateY(-0.5 * delta);
    }
    if (keys.t) {
        step3.rotateY(0.5 * delta);
    }
    if (keys.y) {
        step3.rotateY(-0.5 * delta);
    }
    if (keys.a) {
        if (basic) {
            changeMaterial();
            basic = false;
        } else {
            switch (currentMaterial) {
                case "phong":
                    changeMaterial("lambert");
                    break;
                case "lambert":
                    changeMaterial("phong");
                    break;
            }
        }
        keys.a = false;
    }
    if (keys.s) {
        if (!basic) {
            changeMaterial("basic");
            basic = true;
        } else {
            changeMaterial();
            basic = false;
        }
        keys.s = false;
    }
    if (keys.d) {
        toggleLight(directionalLight);
        keys.d = false;
    }
    if (keys.z) {
        toggleLight(spotLight1);
        keys.z = false;
    }
    if (keys.x) {
        toggleLight(spotLight2);
        keys.x = false;
    }
    if (keys.c) {
        toggleLight(spotLight3);
        keys.c = false;
    }
    if (keys.reset) {
        activeCamera = 1;
        step1.rotation.set(0, 0, 0);
        step2.rotation.set(0, 0, 0);
        step3.rotation.set(0, 0, 0);
        changeMaterial("phong");
        directionalLight.intensity = 0.5;
        spotLight1.children[0].intensity = 1;
        spotLight2.children[0].intensity = 1;
        spotLight3.children[0].intensity = 1;
        paused = false;
        keys.reset = false;
    }
    
    switch (activeCamera) {
        case 1:
            render(perspectiveCamera);
            break;
        case 2:
            render(ortographicCamera);
            break;
    }
}

function render(camera) {
    'use strict';

    renderer.render(scene, camera);
}