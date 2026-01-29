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
//<ID> 1241
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *parsed = cJSON_Parse("{\"name\":\"orig\",\"value\":42}");
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *s = cJSON_CreateString("initial");
    cJSON_AddItemToArray(arr, s);

    // step 2: Configure
    cJSON *nameItem = cJSON_GetObjectItem(parsed, "name");
    char *newname = cJSON_SetValuestring(nameItem, "changed");
    cJSON *created = cJSON_CreateString(newname);
    cJSON_AddItemToObject(root, "name", created);

    // step 3: Operate and Validate
    cJSON_bool is_str = cJSON_IsString(nameItem);
    double computed = 100.0 + (double)is_str;
    cJSON *num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", num);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}