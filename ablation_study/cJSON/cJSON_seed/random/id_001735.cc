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
//<ID> 1735
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *list = (cJSON *)0;
    cJSON *name = (cJSON *)0;
    cJSON *retrieved_meta = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    cJSON_bool has_meta = 0;
    cJSON_bool meta_is_object = 0;
    int list_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    list = cJSON_CreateArray();
    name = cJSON_CreateString("example");

    // step 3: Configure
    cJSON_AddItemToArray(list, cJSON_CreateString("one"));
    cJSON_AddItemToArray(list, cJSON_CreateString("two"));
    cJSON_AddItemToObject(meta, "list", list);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("ok"));

    // step 4: Operate
    has_meta = cJSON_HasObjectItem(root, "meta");
    retrieved_meta = cJSON_GetObjectItem(root, "meta");
    meta_is_object = cJSON_IsObject(retrieved_meta);
    retrieved_list = cJSON_GetObjectItem(retrieved_meta, "list");
    list_size = cJSON_GetArraySize(retrieved_list);

    // step 5: Validate
    validation = (int)has_meta + (int)meta_is_object + (int)(list_size == 2);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)meta;
    (void)list;
    (void)name;
    (void)retrieved_meta;
    (void)retrieved_list;
    (void)has_meta;
    (void)meta_is_object;
    (void)list_size;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}