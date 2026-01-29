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
//<ID> 1910
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
    cJSON *got_num = NULL;
    char *printed = NULL;
    char *set_result = NULL;
    double num_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "label", str_item);

    // step 3: Core operations
    num_item = cJSON_CreateNumber(123.5);
    cJSON_AddItemToObject(root, "count", num_item);
    set_result = cJSON_SetValuestring(str_item, "updated");
    printed = cJSON_PrintUnformatted(root);

    // step 4: Validate
    got_num = cJSON_GetObjectItem(root, "count");
    num_val = cJSON_GetNumberValue(got_num);
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(set_result != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(num_val == 123.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}