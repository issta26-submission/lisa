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
//<ID> 100
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "flag");
    cJSON *s1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(arr, s1);
    cJSON *s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s2);
    cJSON *ins = cJSON_CreateString("inserted");
    cJSON_bool insert_ok = cJSON_InsertItemInArray(arr, 1, ins);
    (void)insert_ok;

    // step 3: Operate
    char *printed = cJSON_Print(root);
    size_t plen = printed ? strlen(printed) : 0;
    char *mutable_json = (char *)cJSON_malloc(plen + 1);
    memcpy(mutable_json, printed ? printed : "", plen);
    mutable_json[plen] = '\0';
    cJSON_Minify(mutable_json);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(mutable_json);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}