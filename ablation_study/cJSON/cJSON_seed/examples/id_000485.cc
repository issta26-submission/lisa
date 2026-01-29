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
//<ID> 485
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *s1 = cJSON_CreateString("alpha");
    const char *beta_literal = "beta";
    cJSON *sref = cJSON_CreateStringReference(beta_literal);
    cJSON_AddItemToArray(list, s1);
    cJSON_AddItemToArray(list, sref);
    cJSON *title = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "title", title);

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON_AddStringToObject(root, "owner", "tester");

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(list, 0);
    char *first_val = cJSON_GetStringValue(first);
    cJSON *second = cJSON_GetArrayItem(list, 1);
    char *second_val = cJSON_GetStringValue(second);
    char buf[128];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "first=%s;second=%s", first_val ? first_val : "null", second_val ? second_val : "null");
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool eq_parsed = cJSON_Compare(root, parsed, 1);
    cJSON_bool eq_dup = cJSON_Compare(root, dup, 1);
    (void)buf;
    (void)eq_parsed;
    (void)eq_dup;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}