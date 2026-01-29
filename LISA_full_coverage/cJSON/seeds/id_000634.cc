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
//<ID> 634
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj2 = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj2, "n", 7.0);

    // step 2: Configure
    cJSON *child1 = cJSON_AddObjectToObject(root, "child1");
    cJSON_AddRawToObject(child1, "embedded", "{\"k\":123}");
    cJSON_AddItemToObject(root, "child2", obj2);

    // step 3: Operate
    cJSON_bool is_obj1 = cJSON_IsObject(child1);
    cJSON_bool is_obj2 = cJSON_IsObject(obj2);
    cJSON *dup = cJSON_Duplicate(child1, 1);
    cJSON_bool cmp1 = cJSON_Compare(child1, dup, 1);
    cJSON_bool cmp2 = cJSON_Compare(child1, obj2, 0);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)is_obj1;
    (void)is_obj2;
    (void)cmp1;
    (void)cmp2;
    cJSON_free(out);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}