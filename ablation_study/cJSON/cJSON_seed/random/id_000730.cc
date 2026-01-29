#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 730
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *devices = (cJSON *)0;
    cJSON *dev1 = (cJSON *)0;
    cJSON *dev2 = (cJSON *)0;
    cJSON *name1 = (cJSON *)0;
    cJSON *id1 = (cJSON *)0;
    cJSON *name2 = (cJSON *)0;
    cJSON *id2 = (cJSON *)0;
    cJSON *retrieved_devices = (cJSON *)0;
    cJSON *detached_devices = (cJSON *)0;
    cJSON_bool added_devices = 0;
    cJSON_bool added_name1 = 0;
    cJSON_bool added_id1 = 0;
    cJSON_bool added_name2 = 0;
    cJSON_bool added_id2 = 0;
    cJSON_bool appended_dev1 = 0;
    cJSON_bool appended_dev2 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    devices = cJSON_CreateArray();
    dev1 = cJSON_CreateObject();
    name1 = cJSON_CreateString("sensor");
    id1 = cJSON_CreateNumber(101.0);
    dev2 = cJSON_CreateObject();
    name2 = cJSON_CreateString("actuator");
    id2 = cJSON_CreateNumber(202.0);

    // step 3: Configure
    added_devices = cJSON_AddItemToObject(root, "devices", devices);
    added_name1 = cJSON_AddItemToObject(dev1, "name", name1);
    added_id1 = cJSON_AddItemToObject(dev1, "id", id1);
    added_name2 = cJSON_AddItemToObject(dev2, "name", name2);
    added_id2 = cJSON_AddItemToObject(dev2, "id", id2);
    appended_dev1 = cJSON_AddItemToArray(devices, dev1);
    appended_dev2 = cJSON_AddItemToArray(devices, dev2);

    // step 4: Operate
    retrieved_devices = cJSON_GetObjectItem(root, "devices");
    detached_devices = cJSON_DetachItemFromObject(root, "devices");
    cJSON_AddItemToObject(root, "restored_devices", detached_devices);
    cJSON_AddItemToArray(detached_devices, cJSON_CreateObject());

    // step 5: Validate
    validation_score = (int)(retrieved_devices != (cJSON *)0)
                     + (int)(detached_devices != (cJSON *)0)
                     + (int)(cJSON_GetObjectItem(root, "restored_devices") != (cJSON *)0)
                     + (int)(cJSON_GetObjectItem(root, "devices") == (cJSON *)0)
                     + (int)added_devices + (int)added_name1 + (int)added_id1 + (int)added_name2 + (int)added_id2
                     + (int)appended_dev1 + (int)appended_dev2;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}