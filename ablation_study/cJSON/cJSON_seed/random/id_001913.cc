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
//<ID> 1913
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
    cJSON *got_num = NULL;
    char *old_valuestring = NULL;
    double num_val = 0.0;
    cJSON_bool add_ok = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.45);
    old_valuestring = cJSON_SetValuestring(num_item, "one two three");
    add_ok = cJSON_AddItemToObject(root, "pi_like", num_item);

    // step 3: Core operations
    got_num = cJSON_GetObjectItem(root, "pi_like");
    num_val = cJSON_GetNumberValue(got_num);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(old_valuestring != NULL);
    validation ^= (int)(add_ok != 0);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(cJSON_IsNumber(got_num));
    validation ^= (int)(num_val == 123.45);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}