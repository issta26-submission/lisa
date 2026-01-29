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
//<ID> 1522
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *values = NULL;
    cJSON *name = NULL;
    cJSON *name_ref = NULL;
    const char *name_val = NULL;
    const char *ver = NULL;
    int arr_size = 0;
    char json[] = "{ \"name\": \"Widget\", \"values\": [10, 20, 30] }";
    size_t len = sizeof(json) - 1;
    char prebuf[64];
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, len);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);

    // step 3: Operate / Validate
    values = cJSON_GetObjectItem(parsed, "values");
    arr_size = cJSON_GetArraySize(values);
    name = cJSON_GetObjectItem(parsed, "name");
    name_val = cJSON_GetStringValue(name);
    name_ref = cJSON_CreateStringReference(name_val);
    cJSON_AddItemToObject(root, "name_ref", name_ref);
    ver = cJSON_Version();
    (void)arr_size;
    (void)ver;
    (void)prebuf;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}