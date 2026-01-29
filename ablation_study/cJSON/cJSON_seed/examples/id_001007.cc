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
//<ID> 1007
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *names = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(names, s1);
    cJSON_AddItemToArray(names, s2);
    cJSON_AddItemToArray(names, s3);
    cJSON_AddItemToObject(root, "names", names);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON *owner = cJSON_CreateString("owner-xyz");
    cJSON_AddItemToObject(meta, "owner", owner);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate & Validate
    int n = cJSON_GetArraySize(names);
    cJSON *second = cJSON_GetArrayItem(names, 1);
    char *second_name = cJSON_GetStringValue(second);
    cJSON *second_copy = cJSON_CreateString(second_name);
    cJSON_AddItemToObject(root, "second_name_copy", second_copy);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "count_plus_one", count_val + 1.0);
    (void)n;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}