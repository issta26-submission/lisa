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
//<ID> 1526
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *meta = NULL;
    cJSON *name = NULL;
    cJSON *second = NULL;
    cJSON *num_cmp = NULL;
    cJSON *detached = NULL;
    cJSON *dup_root = NULL;
    cJSON_bool cmp_result = 0;
    const char *ver = NULL;
    const char *name_str = NULL;
    char json[] = "{\"array\":[10,20,30],\"meta\":{\"name\":\"test_name\"}}";
    size_t len = sizeof(json) - 1;
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup / Configure
    ver = cJSON_Version();
    parsed = cJSON_ParseWithLength(json, len);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);

    // step 3: Operate / Validate
    array = cJSON_GetObjectItem(parsed, "array");
    int count = cJSON_GetArraySize(array);
    second = cJSON_GetArrayItem(array, 1);
    num_cmp = cJSON_CreateNumber(20);
    cmp_result = cJSON_Compare(second, num_cmp, 1);
    meta = cJSON_GetObjectItem(parsed, "meta");
    name = cJSON_GetObjectItem(meta, "name");
    name_str = cJSON_GetStringValue(name);
    detached = cJSON_DetachItemFromArray(array, 2);
    cJSON_Delete(detached);
    cJSON_Delete(num_cmp);
    dup_root = cJSON_Duplicate(root, 1);
    cJSON_Delete(dup_root);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)ver;
    (void)name_str;
    (void)cmp_result;
    (void)count;
    (void)scratch;
    // API sequence test completed successfully
    return 66;
}