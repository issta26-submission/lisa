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
//<ID> 1143
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *root = cJSON_CreateObject();
    cJSON *str_arr = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObject(root, "strings", str_arr);
    cJSON *childA = cJSON_CreateObject();
    cJSON_AddNumberToObject(childA, "value", 11.0);
    cJSON_AddItemToObject(root, "childA", childA);
    cJSON *childB = cJSON_CreateObject();
    cJSON_AddNumberToObject(childB, "value", 24.0);
    cJSON_AddItemToObject(root, "childB", childB);
    cJSON_AddNumberToObject(root, "temp", 7.0);

    cJSON_AddItemReferenceToObject(root, "childA_ref", childA);
    cJSON_DeleteItemFromObject(root, "temp");

    cJSON *val_b = cJSON_GetObjectItem(childB, "value");
    double num_b = cJSON_GetNumberValue(val_b);
    cJSON_AddNumberToObject(root, "computed", num_b + 1.0);
    cJSON *replacement = cJSON_CreateString("replaced_childB");
    cJSON_ReplaceItemViaPointer(root, childB, replacement);

    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    return 66;
}