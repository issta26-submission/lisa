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
//<ID> 1232
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
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *shared = cJSON_CreateNumber(3.14159);
    cJSON_AddItemReferenceToObject(root, "shared", shared);
    cJSON_AddItemReferenceToArray(arr, shared);
    cJSON *temp = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, temp);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromArray(arr, 0);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buf = (char *)cJSON_malloc(printed_len + 16);
    memset(buf, 0, printed_len + 16);
    memcpy(buf, printed ? printed : "", printed_len + 1);
    cJSON_Minify(buf);
    cJSON *got_shared = cJSON_GetObjectItem(root, "shared");
    double shared_val = cJSON_GetNumberValue(got_shared);
    (void)shared_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}