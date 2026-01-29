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
//<ID> 1961
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
    cJSON *bool_item = NULL;
    cJSON *str_item = NULL;
    cJSON *got_num = NULL;
    void *ext_mem = NULL;
    double num_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "num", 42.5);
    bool_item = cJSON_AddBoolToObject(root, "flag", 1);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 3: Core operations
    got_num = cJSON_GetObjectItem(root, "num");
    num_val = cJSON_GetNumberValue(got_num);
    ext_mem = cJSON_malloc(8);
    ((char *)ext_mem)[0] = (char)((int)num_val);
    buf[0] = ((char *)ext_mem)[0];

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(ext_mem != NULL);
    validation ^= (int)(num_val == 42.5);
    buf[1] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(ext_mem);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}