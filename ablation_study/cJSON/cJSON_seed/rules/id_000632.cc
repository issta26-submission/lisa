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
//<ID> 632
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
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *s3 = cJSON_CreateString("three");
    cJSON *second = NULL;
    char *sval = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    size_t sval_len = 0;
    size_t copy2 = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate / Validate
    second = cJSON_GetArrayItem(arr, 1);
    sval = cJSON_GetStringValue(second);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(scratch, printed ? printed : "", to_copy);
    sval_len = sval ? strlen(sval) : 0;
    copy2 = sval_len < (255 - to_copy) ? sval_len : (255 - to_copy);
    memcpy(scratch + to_copy, sval ? sval : "", copy2);
    scratch[to_copy + copy2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}