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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *other = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    char *printed_root = NULL;
    char *printed_other = NULL;
    cJSON_bool equal = 0;
    double extracted = 0.0;
    double score = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "answer", num);
    cJSON_AddItemToObject(root, "meta", raw);
    cJSON_AddNumberToObject(other, "answer", 42.0);
    cJSON_AddRawToObject(other, "meta", "{\"raw\":true}");

    // step 3: Operate / Validate
    equal = cJSON_Compare(root, other, 1);
    extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));
    score = extracted + (double)equal;
    cJSON_AddNumberToObject(root, "score", score);
    printed_root = cJSON_PrintUnformatted(root);
    printed_other = cJSON_PrintUnformatted(other);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_other);
    cJSON_Delete(root);
    cJSON_Delete(other);

    // API sequence test completed successfully
    return 66;
}