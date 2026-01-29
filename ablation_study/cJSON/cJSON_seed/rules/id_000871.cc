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
//<ID> 871
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *got = NULL;
    char *printed = NULL;
    void *tmp_mem = NULL;
    char *key_copy = NULL;
    double val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num1 = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", num1);
    num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num2);
    tmp_mem = cJSON_malloc(16);
    key_copy = (char *)tmp_mem;
    memset(key_copy, 0, 16);
    memcpy(key_copy, "answer", 7);

    // step 3: Operate / Validate
    got = cJSON_GetObjectItem(root, "pi");
    val = cJSON_GetNumberValue(got);
    printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObjectCaseSensitive(root, key_copy);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_mem);
    cJSON_Delete(root);
    (void)val;
    (void)got;

    // API sequence test completed successfully
    return 66;
}