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
//<ID> 1234
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
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON *dup = cJSON_Duplicate(num, 1);
    cJSON_AddItemReferenceToArray(arr, num);
    cJSON_AddItemReferenceToObject(root, "number_copy", dup);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromArray(arr, 0);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buf = (char *)cJSON_malloc(printed_len + 16);
    memset(buf, 0, printed_len + 16);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON *found = cJSON_GetObjectItem(root, "number_copy");
    double retrieved_value = cJSON_GetNumberValue(found);
    (void)retrieved_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}