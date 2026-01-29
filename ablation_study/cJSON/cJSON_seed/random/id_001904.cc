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
//<ID> 1904
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
    cJSON *num_item1 = NULL;
    cJSON *num_read1 = NULL;
    cJSON *num_read2 = NULL;
    cJSON *bool_read = NULL;
    char *out = NULL;
    double val1 = 0.0;
    double val2 = 0.0;
    cJSON_bool flag_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item1 = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi_explicit", num_item1);
    cJSON_AddNumberToObject(root, "e_convenience", 2.71828);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Core operations
    num_read1 = cJSON_GetObjectItem(root, "pi_explicit");
    val1 = cJSON_GetNumberValue(num_read1);
    num_read2 = cJSON_GetObjectItem(root, "e_convenience");
    val2 = cJSON_GetNumberValue(num_read2);
    bool_read = cJSON_GetObjectItem(root, "enabled");
    flag_true = cJSON_IsTrue(bool_read);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item1 != NULL);
    validation ^= (int)(num_read1 != NULL);
    validation ^= (int)(num_read2 != NULL);
    validation ^= (int)(bool_read != NULL);
    validation ^= (int)(val1 > 3.0);
    validation ^= (int)(val2 > 2.0);
    validation ^= (int)(flag_true != 0);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}