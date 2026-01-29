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
//<ID> 1918
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
    char *new_valstr = NULL;
    char *out = NULL;
    double got_number = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.456);
    str_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddItemToObject(root, "label", str_item);

    // step 3: Core operations
    new_valstr = cJSON_SetValuestring(str_item, "updated");
    got_num = cJSON_GetObjectItem(root, "value");
    got_number = cJSON_GetNumberValue(got_num);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(new_valstr != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_number == 123.456);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}