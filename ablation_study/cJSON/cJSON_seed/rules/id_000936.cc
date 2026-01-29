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
//<ID> 936
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *second_item = NULL;
    cJSON *retrieved_second = NULL;
    cJSON *fourth_item = NULL;
    char *printed_before = NULL;
    char *printed_after = NULL;
    char *scratch = NULL;
    const char *val1 = "alpha";
    const char *val2 = "beta";
    const char *val3 = "gamma";
    cJSON_bool second_is_invalid = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "first", val1);
    second_item = cJSON_CreateString(val2);
    cJSON_AddItemToObject(root, "second", second_item);
    cJSON_AddStringToObject(root, "third", val3);

    // step 3: Operate / Validate
    printed_before = cJSON_PrintUnformatted(root);
    retrieved_second = cJSON_GetObjectItem(root, "second");
    second_is_invalid = cJSON_IsInvalid(retrieved_second);
    cJSON_DeleteItemFromObject(root, "first");
    fourth_item = cJSON_CreateString("delta");
    cJSON_AddItemToObject(root, "fourth", fourth_item);
    printed_after = cJSON_PrintUnformatted(root);
    scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    scratch[0] = (printed_before && printed_before[0]) ? printed_before[0] : 'X';
    scratch[1] = (printed_after && printed_after[0]) ? printed_after[0] : 'Y';
    scratch[2] = (char)('0' + (second_is_invalid ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(printed_before);
    cJSON_free(printed_after);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}