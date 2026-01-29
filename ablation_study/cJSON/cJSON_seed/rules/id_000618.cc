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
//<ID> 618
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *meta_copy = NULL;
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    size_t printed_len = 0;
    int array_size = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(meta, "count", 2.0);
    meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_original", meta);
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToObject(root, "values", array);
    cJSON_AddRawToObject(root, "raw", "{\"note\":\"hello\",\"v\":123}");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    array_size = cJSON_GetArraySize(array);
    memcpy(scratch, printed ? printed : "", printed_len < 511 ? printed_len : 511);
    scratch[printed_len < 511 ? printed_len : 511] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}