const origamiTexture = new THREE.TextureLoader().load('textures/origami.jpg');

const frontSimpleMaterial = new THREE.MeshPhongMaterial({ color: 0xadd8e6, side: THREE.FrontSide, shininess: 0 });
const backSimpleMaterial = new THREE.MeshPhongMaterial({ color: 0xadd8e6, side: THREE.BackSide, shininess: 0 });
const doubleSimpleMaterial = new THREE.MeshPhongMaterial({ color: 0xadd8e6, side: THREE.DoubleSide, shininess: 0 });
const frontOrigamiMaterial = new THREE.MeshPhongMaterial({ map: origamiTexture, side: THREE.FrontSide, shininess: 0 });
const backOrigamiMaterial = new THREE.MeshPhongMaterial({ map: origamiTexture, side: THREE.BackSide, shininess: 0 });
const doubleOrigamiMaterial = new THREE.MeshPhongMaterial({ map: origamiTexture, side: THREE.DoubleSide, shininess: 0 });


function createMultiMaterialObject(geometry, materials) {
    'use strict';

	const group = new THREE.Group();

	for (let i = 0; i < materials.length; i ++) {
        let mesh = new THREE.Mesh(geometry, materials[i]);
        mesh.castShadow = true;
		group.add(mesh);
	}

	return group;
}

function createStep1(x, y, z) {
    'use strict';

    const positions = [
        40, 2, 0,
        0, 0, 40,
        0, 0, -40,
    ];

    const uvs = [
        1, 0,
        1, 1, 
        0, 0,
    ];

    let materials = [
        backSimpleMaterial, 
        frontOrigamiMaterial
    ];
    
    let step = new THREE.Group();

    let geometry = new THREE.BufferGeometry();
    geometry.setAttribute('position', new THREE.Float32BufferAttribute(positions, 3));
    geometry.setAttribute('uv', new THREE.Float32BufferAttribute(uvs, 2));
    geometry.computeVertexNormals();

    let right = createMultiMaterialObject(geometry, materials);
    
    right.rotation.set(Math.PI/3, 0, 0);
    step.add(right)

    let left = right.clone();
    left.scale.set(-1, 1, 1);
    step.add(left)
    
    step.position.set(x, y, z);
    step.scale.set(0.7, 0.7, 0.7)
    objects.push(step);
    scene.add(step);
    return step;
}

function createStep2(x, y, z) {
    'use strict';

    const positions = [
        [10, 0, -30,
        0, 0, 40,
        0, 0, -40], //frente1

        [10, 0, -30,
        0, 0, 40,
        0.2, 1, -24],//frente2

        [8.6, 1.01, -20,
        0, 0, 40,
        0.2, 1, -24,],

        [9.2, 0, -24,
        0, 0, 40,
        0.2, -1, -24]
    ];

    const uvs = [
        [0.6, 1,
        0, 0,
        1, 1],
        [0.38, 1, 
        0, 0, 
        0.18, 1],
        [0.18, 1,
        0, 0,
        0.28, 1,],
        [0, 1,
        0, 0,
        0.18, 1]
    ];

    let materials = [
        frontSimpleMaterial,
        backOrigamiMaterial
    ];  

    let step = new THREE.Group();

    let right = new THREE.Object3D();
    for (let index = 0; index < positions.length; index++) {
        let geometry = new THREE.BufferGeometry();
        geometry.setAttribute('position', new THREE.Float32BufferAttribute(positions[index], 3));
        geometry.setAttribute('uv', new THREE.Float32BufferAttribute(uvs[index], 2));
        geometry.computeVertexNormals();
        let piece;
        if (index == 0) {
            piece = createMultiMaterialObject(geometry, materials);
        } else if (index == 2 || index == 3) {
            piece = new THREE.Mesh(geometry, doubleOrigamiMaterial);
        } else {
            piece = new THREE.Mesh(geometry, doubleSimpleMaterial);
        }
        piece.castShadow = true;
        right.add(piece);
    }
    right.rotation.set(Math.PI/3, 0, 0);
    step.add(right);
    
    let left = right.clone();
    left.scale.set(-1, 1, 1);
    step.add(left);

    step.position.set(x, y, z);
    step.scale.set(0.7, 0.7, 0.7);
    objects.push(step);
    scene.add(step);
    return step;
}

function createStep3(x, y, z) {
    'use strict';

    const positions = [
        [40, 0, -10, //D
        8, 0, -3.5, //E
        20, 6, 20],//F
        [0, 4, 20, //G
        20, 6, 20,   //H
        8, 0, -3.5],//I
        [0, 4, 20, //M
        -30, 0, 5,   //N
        8, 0, -3.5],//O
        [0, 4, 20, //P
        -30, 0, 5,   //Q
        -20, 2, 20],//R
        [-20, 2, 20,  
        -30, 0, 5,   
        -20, 4, 15], 
        [-30, 0, 5,
        -20, 4, 15,
        0, 2, -30],
        [-30, 0, 5,
        -2, 0, -32,
        0, 2, -30],
        [0, 2, -30,
        -2, 0, -32,
        -20, 0, -26], 
    ];

    const uvs = [
        [1, 1, //D
        0.78, 0.78, //E
        1, 0.7],//F
        [0, 0, //G
        0, 0,   //H
        0, 0,],//I
        [0.2, 1, //M
        0.25, 0.65,   //N
        0.4, 1],//O
        [0.2, 1, //P
        0.25, 0.65,//Q
        0.15, 0.8],//R
        [0.15, 0.8, 
        0, 0.7,   
        0.1, 0.7,], 
        [0.1, 0.7,
        0, 0.7,
        0.05, 0.3],
        [0.1, 0.7,
        0, 0.28,
        0.05, 0.3],
        [0.05, 0.3,
        0, 0.28,
        0, 0], 
    ];

    let materials = [
        backSimpleMaterial,
        frontOrigamiMaterial
    ];

    let step = new THREE.Group();

    let right = new THREE.Object3D();
    for (let index = 0; index < positions.length; index++) {
        let geometry = new THREE.BufferGeometry();
        geometry.setAttribute('position', new THREE.Float32BufferAttribute(positions[index], 3));
        geometry.setAttribute('uv', new THREE.Float32BufferAttribute(uvs[index], 2));
        geometry.computeVertexNormals();
        let piece;
        if (index == 1) {
            piece = new THREE.Mesh(geometry, doubleSimpleMaterial);
        } else if (index == 0) {
            piece = createMultiMaterialObject(geometry, materials);
        } else {
            piece = new THREE.Mesh(geometry, doubleOrigamiMaterial);
        }
        piece.castShadow = true;
        right.add(piece);
    }
    right.rotation.set(Math.PI/2, 0, 0);
    step.add(right);
    
    let left = right.clone();
    left.scale.set(1, -1, 1);
    step.add(left);

    step.position.set(x, y, z);
    step.scale.set(0.7, 0.7, 0.7);

    objects.push(step);
    scene.add(step);
    return step;
}

function changeMaterial(material=currentMaterial) {
    for (let i = 0; i < objects.length; i++) {
        changeMaterialAux(objects[i], material);
    }
    if (material != "basic") {
        currentMaterial = material;
    }
}

function changeMaterialAux(object, material) {
    'use strict';

    if (object instanceof THREE.Mesh) {
        let newMaterial
        if (object.material.map == null) {
            if (material === "basic")
                newMaterial = new THREE.MeshBasicMaterial({color: object.material.color, side: object.material.side});
            else if (material === "phong")
                newMaterial = new THREE.MeshPhongMaterial({color: object.material.color, side: object.material.side, shininess: 0});
            else if (material === "lambert")
                newMaterial = new THREE.MeshLambertMaterial({color: object.material.color, side: object.material.side});
        } else {
            if (material === "basic")
                newMaterial = new THREE.MeshBasicMaterial({map: object.material.map, side: object.material.side});
            else if (material === "phong")
                newMaterial = new THREE.MeshPhongMaterial({map: object.material.map, side: object.material.side, shininess: 0});
            else if (material === "lambert")
                newMaterial = new THREE.MeshLambertMaterial({map: object.material.map, side: object.material.side});
        }
        object.material.dispose();
        object.material = newMaterial;
    } else {
        for (let i = 0; i < object.children.length; i++) {
            changeMaterialAux(object.children[i], material)
        }
    }
}