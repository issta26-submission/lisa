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
//<ID> 1967
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *bool_item = NULL;
    char *heap_buf = NULL;
    double num_val = 0.0;
    cJSON_bool added_bool = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greet", str_item);

    // step 3: Core operations
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);
    num_item = cJSON_GetObjectItem(root, "pi");
    num_val = cJSON_GetNumberValue(num_item);
    heap_buf = (char *)cJSON_malloc(64);
    memset(heap_buf, 0, 64);
    heap_buf[0] = (char)('A' + (int)(num_val) % 26); /* use heap_buf to hold a derived value */

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(heap_buf != NULL);
    validation ^= (int)(num_val == 3.1415);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(heap_buf);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}