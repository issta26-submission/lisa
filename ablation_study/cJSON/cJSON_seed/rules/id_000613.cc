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
//<ID> 613
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed_root = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON *dup_root = NULL;
    size_t to_copy = 0;
    size_t printed_len = 0;
    int count = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToArray(numbers, n3);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddRawToObject(root, "raw", "{\"status\":\"ok\",\"v\":10}");

    // step 3: Operate / Validate
    count = cJSON_GetArraySize(numbers);
    cJSON_AddNumberToObject(root, "count", (double)count);
    dup_root = cJSON_Duplicate(root, 1);
    printed_root = cJSON_PrintUnformatted(dup_root);
    printed_len = printed_root ? strlen(printed_root) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed_root ? printed_root : "", to_copy);
    scratch[to_copy] = '\0';

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(scratch);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}