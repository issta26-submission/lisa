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
//<ID> 1851
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
    cJSON *nested = NULL;
    cJSON *num = NULL;
    cJSON *num2 = NULL;
    cJSON *false_item = NULL;
    char *out = NULL;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", nested);
    num2 = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(nested, "pi", num2);

    // step 3: Core operations
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(nested != NULL);
    validation ^= (int)(num != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}