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
//<ID> 1915
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
    cJSON *got_number = NULL;
    double num_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();

    // step 3: Core operations
    num_item = cJSON_CreateNumber(7.25);
    cJSON_SetValuestring(num_item, "seven point two five");
    cJSON_AddItemToObject(root, "value", num_item);
    got_number = cJSON_GetObjectItem(root, "value");
    num_val = cJSON_GetNumberValue(got_number);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(got_number != NULL);
    validation ^= (int)(num_val == 7.25);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}