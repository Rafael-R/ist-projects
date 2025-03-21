/* global THREE */

const radius = 48;
const viewSize = radius * 3;
const spaceshipHeight = radius / 11;
const n_trashes = 40;

const backgroundTexture = new THREE.TextureLoader().load('https://images.unsplash.com/photo-1520034475321-cbe63696469a?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8&auto=format&fit=crop&w=2070&q=80');
const planetTexture = new THREE.TextureLoader().load('http://1.bp.blogspot.com/_9DvzmslTIME/TMoOp-gpD6I/AAAAAAAAAdY/xW2XCiOgS9Q/s1600/Planet_2_d.png');
const trashTexture = new THREE.TextureLoader().load('https://images-wixmp-ed30a86b8c4ca887773594c2.wixmp.com/f/e0763947-6f42-4d09-944f-c2d6f41c415b/dcaig77-18800e1e-24aa-43e5-9dd0-3dff9bcf8d0c.jpg?token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJ1cm46YXBwOjdlMGQxODg5ODIyNjQzNzNhNWYwZDQxNWVhMGQyNmUwIiwiaXNzIjoidXJuOmFwcDo3ZTBkMTg4OTgyMjY0MzczYTVmMGQ0MTVlYTBkMjZlMCIsIm9iaiI6W1t7InBhdGgiOiJcL2ZcL2UwNzYzOTQ3LTZmNDItNGQwOS05NDRmLWMyZDZmNDFjNDE1YlwvZGNhaWc3Ny0xODgwMGUxZS0yNGFhLTQzZTUtOWRkMC0zZGZmOWJjZjhkMGMuanBnIn1dXSwiYXVkIjpbInVybjpzZXJ2aWNlOmZpbGUuZG93bmxvYWQiXX0.TLHMNUPdW5unEpvHhX2wn8iRLpHN15hcMvFLWtmXWyE');

var renderer, scene, clock, delta;
var activeCamera, ortographicCamera, perspectiveCamera, spaceshipCamera;
var system, spaceship, trashes = [[], [], [], []];

let keys = {
    arrowLeft: false,
    arrowUp: false,
    arrowRight: false,
    arrowDown: false,
    add: false,
    sub: false
};


function createBox(texture, width, height, depth) {
    'use strict';

    let geometry = new THREE.BoxGeometry(width, height, depth);
    let material = new THREE.MeshBasicMaterial({ map: texture });
    var box = new THREE.Mesh(geometry, material);

    return box;
}

function createCone(texture, radius, height, radialSegments, heightSegments) {
    'use strict';

    let geometry = new THREE.ConeGeometry(radius, height, radialSegments, heightSegments);
    let material = new THREE.MeshBasicMaterial({ map: texture });
    let cone = new THREE.Mesh(geometry, material);

    return cone;
}

function createSphere(texture, radius, widthSegments, heightSegments, phiStart, phiLenght, thetaStart) {
    'use strict';

    let geometry = new THREE.SphereGeometry(radius, widthSegments, heightSegments, phiStart, phiLenght, thetaStart);
    let material = new THREE.MeshBasicMaterial({ map: texture });
    let sphere = new THREE.Mesh(geometry, material);

    return sphere;
}

function createCapsule(color, radius, length, capSegments, heightSegments) {
    'use strict';

    let geometry = new THREE.CapsuleGeometry(radius, length, capSegments, heightSegments);
    let material = new THREE.MeshBasicMaterial({ color: color });
    let capsule = new THREE.Mesh(geometry, material);

    return capsule;
}

function createCylinder(color, radiusTop, radiusBottom, height, radialSegments) {
    'use strict';

    let geometry = new THREE.CylinderGeometry(radiusTop, radiusBottom, height, radialSegments);
    let material = new THREE.MeshBasicMaterial({ color: color });
    let cylinder = new THREE.Mesh(geometry, material);

    return cylinder;
}

function getQuadrant(object) {
    'use strict';
    
    var x = object.position.x
    var y = object.position.y;

    if (x >= 0 && y >= 0) {
        return 0;
    } else if (x >= 0 && y < 0) {
        return 1;
    } else if (x < 0 && y >= 0) {
        return 2;
    } else {
        return 3;
    }      
}

function setPosition(object, radius, phi, theta) {
    'use strict';

    object.position.x = radius * Math.sin(phi) * Math.sin(theta);
    object.position.y = radius * Math.cos(phi);
    object.position.z = radius * Math.sin(phi) * Math.cos(theta);
}

function createTrash(radius) {
    'use strict';

    let type, size1, size2, object, diagonal;

    for (let index = 0; index < n_trashes; index++) {
        
        type = Math.ceil(Math.random() * 2);    // # of different shapes

        size1 = radius / 20 + Math.random() * (radius / 21 - radius / 23);
        
        switch (type) {
            case 1:
                size2 = radius / 20 + Math.random() * (radius / 21 - radius / 23);
                object = createBox(trashTexture, size1, size1, size2);
                diagonal = Math.sqrt(2*(size1**2) + size2**2)
                object.userData.radius = diagonal/2;
                break;
            case 2:
                object = createSphere(trashTexture, size1 / 2, Math.random() * 8 , Math.random() * 8);
                object.userData.radius = size1 / 2;
                break;
        }

        object.rotation.x = Math.random() * Math.PI * 2;
        object.rotation.y = Math.random() * Math.PI * 2;
        object.rotation.z = Math.random() * Math.PI * 2;
        setPosition(object, 1.2 * radius, Math.random() * Math.PI * 2, Math.random() * Math.PI * 2);
                     
        trashes[getQuadrant(object)].push(object);                                              
    }                                          
}                                 

function createSpaceship() {
    'use strict';

    let spaceship = new THREE.Object3D();
    
    let body = createCylinder(0xbbbbbb, 1, 1.5, 4, 6);

    let nose = createCylinder(0xa82f2f, 0.25, 1, 1, 6);
    nose.position.set(0, 2 + 0.5, 0);

    let thruster1 = createCapsule(0xa82f2f, 0.5, 0.5, 1, 20);
    let thruster2 = thruster1.clone();
    let thruster3 = thruster1.clone();
    let thruster4 = thruster1.clone();
    thruster1.position.set(1, -2 + 0.5, 1);
    thruster2.position.set(1, -2 + 0.5, -1);
    thruster3.position.set(-1, -2 + 0.5, 1);
    thruster4.position.set(-1, -2 + 0.5, -1);

    spaceship.add(body);
    spaceship.add(nose);
    spaceship.add(thruster1);
    spaceship.add(thruster2);
    spaceship.add(thruster3);
    spaceship.add(thruster4);

    let box = new THREE.Box3().setFromObject(spaceship);
    let height = box.max.y - box.min.y;         // current height
    let scale = spaceshipHeight / height;       // new height = spaceshipHeight
    spaceship.scale.set(scale, scale, scale);   // fix scale

    return spaceship;
}

function updateSpaceship() {
    'use strict';

    const forward = new THREE.Vector3(0, 1, 0);
    const reverse = new THREE.Vector3(0, -1, 0);

    setPosition(spaceship, 1.2 * radius, spaceship.userData.phi, spaceship.userData.theta);

    (Math.floor(spaceship.userData.phi / Math.PI) % 2 == 1) ? spaceship.up = forward : spaceship.up = reverse;

    spaceship.lookAt(0, 0, 0);
}

function createSystem() {
    'use strict';

    system = new THREE.Object3D();
    system.userData = { scale: 1 };

    let planet = createSphere(planetTexture, radius, radius, radius);

    spaceship = new THREE.Group();
    let spaceshipObject = createSpaceship();
    spaceshipCamera = createCamera("perspective", 0, -5, spaceshipObject.position.z - 10);
    spaceshipCamera.lookAt(planet.position);
    spaceship.add(spaceshipObject);
    spaceship.add(spaceshipCamera);
    spaceship.userData = { radius: spaceshipHeight / 2,             // random starting position
                           phi: Math.random() * Math.PI * 2, 
                           theta: Math.random() * Math.PI * 2 };
    updateSpaceship();

    createTrash(radius);

    system.add(planet);
    system.add(spaceship);
    trashes.forEach(quadrant => {
        quadrant.forEach(trash => {
            system.add(trash);
        });
    });

    scene.add(system);
}

function createScene() {
    'use strict';

    scene = new THREE.Scene();

    createSystem();
}

function createCamera(type, x, y, z) {
    'use strict';

    let camera;

    let aspect = window.innerWidth / window.innerHeight;
    
    switch (type) {
        case "ortographic":
            camera = new THREE.OrthographicCamera(viewSize * aspect / -2,
                                                  viewSize * aspect / 2,
                                                  viewSize / 2,
                                                  viewSize / -2,
                                                  1,
                                                  1000);
            break;
        case "perspective":
            camera = new THREE.PerspectiveCamera(70, aspect, 1, 1000);
            break;
    }

    camera.position.set(x, y, z);

    return camera;
}

function onKeyDown(e) {
    'use strict';

    switch (e.keyCode) {
        case 49:    // 1
            activeCamera = 1;
            break;
        case 50:    // 2
            activeCamera = 2;
            break;
        case 51:    // 3
            activeCamera = 3;
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

function resizeCamera(camera, type) {
    'use strict';

    let aspect = window.innerWidth / window.innerHeight;

    switch (type) {
        case "ortographic":
            camera.left = viewSize * aspect / -2;
            camera.right = viewSize * aspect / 2;
            camera.top = viewSize / 2;
            camera.bottom = viewSize / -2;
            break;
        case "perspective":
            camera.aspect = aspect;
            break;
    }

    camera.updateProjectionMatrix();
}

function onWindowResize() {
    'use strict';

    resizeCamera(ortographicCamera, "ortographic");
    resizeCamera(perspectiveCamera, "perspective");
    resizeCamera(spaceshipCamera, "perspective");

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
    scene.background = backgroundTexture;

    ortographicCamera = createCamera("ortographic", 0, 0, 120);
    ortographicCamera.lookAt(scene.position);
    perspectiveCamera = createCamera("perspective", 0, 0, 120);
    perspectiveCamera.lookAt(scene.position);
    activeCamera = 1;

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
    
    let quadrant = getQuadrant(spaceship);
    for (let index = trashes[quadrant].length - 1; index >= 0 ; index--) {      
        let trash = trashes[quadrant][index];
        if (spaceship.userData.radius + trash.userData.radius > spaceship.position.distanceTo(trash.position)) {
            system.remove(trash);
            trashes[quadrant].splice(index, 1);
        }     
    }           
    
    if (keys.arrowLeft) {
        if (Math.floor(spaceship.userData.phi / Math.PI) % 2 == 0) {
            spaceship.userData.theta += 0.3 * delta;
        } else {
            spaceship.userData.theta -= 0.3 * delta;
        }
        updateSpaceship();
    }
    if (keys.arrowRight) {
        if (Math.floor(spaceship.userData.phi / Math.PI) % 2 == 0) {
            spaceship.userData.theta -= 0.3 * delta;
        } else {
            spaceship.userData.theta += 0.3 * delta;
        }
        updateSpaceship();
    }
    if (keys.arrowUp) {
        spaceship.userData.phi += 0.3 * delta;
        updateSpaceship();
    }
    if (keys.arrowDown) {
        spaceship.userData.phi -= 0.3 * delta;
        updateSpaceship();
    }
    if (keys.add) {
        system.userData.scale += 0.1 * delta;
        system.scale.set(system.userData.scale,
                         system.userData.scale,
                         system.userData.scale);
    }
    if (keys.sub) {
        system.userData.scale -= 0.1 * delta;
        system.scale.set(system.userData.scale,
                         system.userData.scale,
                         system.userData.scale);
    }
    
    switch (activeCamera) {
        case 1:
            render(ortographicCamera);
            break;
        case 2:
            render(perspectiveCamera);
            break;
        case 3:
            render(spaceshipCamera);
            break;
    }
    
    requestAnimationFrame(animate);
}