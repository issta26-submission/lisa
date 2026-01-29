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
//<ID> 2120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *true_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *got_true = NULL;
    void *mem = NULL;
    cJSON_bool is_true = 0;
    double got_value = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateArray();
    mem = cJSON_malloc(16);
    memset(mem, 0, 16);
    ((char *)mem)[0] = 'r';
    ((char *)mem)[1] = 'a';
    ((char *)mem)[2] = 'w';
    ((char *)mem)[3] = '\0';
    raw_item = cJSON_CreateRaw((const char *)mem);

    // step 3: Core operations
    cJSON_AddItemToArray(root, raw_item);
    num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(root, num1);
    num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(root, num2);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(root, true_item);
    got_true = cJSON_GetArrayItem(root, 3);
    is_true = cJSON_IsTrue(got_true);
    got_value = cJSON_GetNumberValue(cJSON_GetArrayItem(root, 2));

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(mem != NULL);
    validation ^= (int)(raw_item != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(got_value == 42.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(mem);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}