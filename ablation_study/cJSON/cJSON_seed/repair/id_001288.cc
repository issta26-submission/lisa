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
//<ID> 1288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_name = "Alice";
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString(initial_name);
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddItemToObject(person, "age", age);
    cJSON_AddItemToObject(root, "person", person);
    cJSON *tags = cJSON_CreateArray();
    cJSON_AddItemToArray(tags, cJSON_CreateString("a"));
    cJSON_AddItemToArray(tags, cJSON_CreateString("b"));
    cJSON_AddItemToArray(tags, cJSON_CreateString("c"));
    cJSON_AddItemToObject(root, "tags", tags);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "note", "created by test");
    cJSON *person_ptr = cJSON_GetObjectItem(root, "person");
    cJSON *name_ptr = cJSON_GetObjectItem(person_ptr, "name");
    cJSON *new_name = cJSON_CreateString("Bob");
    cJSON_ReplaceItemViaPointer(person_ptr, name_ptr, new_name);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}