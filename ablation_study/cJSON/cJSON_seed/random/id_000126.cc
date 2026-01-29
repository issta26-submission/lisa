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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *title = cJSON_CreateString("Example Title");
    cJSON *ratings = (cJSON *)0;
    float scores[3];
    double average = 0.0;
    char *json_text = (char *)0;
    double validated_average = 0.0;

    // step 2: Setup - prepare numeric data
    scores[0] = 4.5f;
    scores[1] = 3.0f;
    scores[2] = 5.0f;

    // step 3: Configure - attach metadata and strings, create and attach float array
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "title", title);
    cJSON_AddStringToObject(meta, "publisher", "OpenPublisher");
    ratings = cJSON_CreateFloatArray(scores, 3);
    cJSON_AddItemToObject(root, "ratings", ratings);

    // step 4: Operate - compute average and add as a number in the root object
    average = (double)(scores[0] + scores[1] + scores[2]) / 3.0;
    cJSON_AddNumberToObject(root, "average", average);

    // step 5: Validate - serialize and read back the stored average
    json_text = cJSON_PrintUnformatted(root);
    validated_average = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "average"));
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}